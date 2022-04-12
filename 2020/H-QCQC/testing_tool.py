#!/usr/bin/env python3

"""
Local testing tool for QC QC.

Disclaimer: This is *not* the same code used to test your solution when it is
submitted. The purpose of this tool is to help with debugging the interactive
problem and it has no ambitions to extensively test all possibilities that
are allowed by the problem statement. While the tool tries to yield the same
results as the real judging system, this is not guaranteed and the result
may differ if the tested program does not use the correct formatting or
exhibits other incorrect behavior. It also *does not* apply the time and
memory limits that are applied to submitted solutions.

The behavior is controlled by an input data file with the following format.
The first line contains `b`, the number of batches. This is followed by
descriptions of `b` batches. The first line of a batch contains `n`,
the number of machines, and the state of those machines as a string
of 0's and 1's (the same format as the answer). The remaining `n` lines each
contain a string of `n` 0's and 1's. The jth number on the ith line contains
the answer given when the ith machine tests the jth machine.

To run the testing tool, run::

    pypy3 testing_tool.py <input file> <program> <arguments>

where `arguments` are optional arguments to the program to run. The following
show examples for different languages::

    pypy3 testing_tool.py test.in ./myprogram
    pypy3 testing_tool.py test.in java -cp . MyProgram
    pypy3 testing_tool.py test.in pypy3 myprogram.py

One can also pass `--verbose` (before the input file name) to see a log of the
complete interaction.
"""

import argparse
import subprocess
import re
import sys
from typing import TextIO


MAX_N = 100
MAX_QUERIES = 12


class WrongAnswer(RuntimeError):
    """Raised whenever an incorrect answer is received."""
    pass


def vprint(*args, verbose: bool, file: TextIO, **kwargs) -> None:
    """Print to `file`, and also to stdout if `verbose is true."""
    if verbose:
        print('< ', end='')
        print(*args, **kwargs)
        sys.stdout.flush()
    print(*args, file=file, flush=True, **kwargs)


def vreadline(data: TextIO, *, verbose: bool) -> str:
    """Read a line from `data`, and also log it to stdout if `verbose` is true."""
    line = data.readline()
    if verbose and line:
        print('>', line.rstrip('\n'))
    return line


class Batch:
    """Input data from a single batch."""

    def __init__(self, data: TextIO) -> None:
        n_str, state = data.readline().split()
        n = int(n_str)
        if not 1 <= n <= MAX_N:
            raise ValueError('n is out of range')
        if len(state) != n:
            raise ValueError('answer string must have length n')
        if not re.fullmatch('[01]+', state):
            raise ValueError('answer string must contain only zeros and ones')
        if state.count('1') * 2 <= n:
            raise ValueError('more than half the machines must be functioning')
        self.n = n
        self.state = state
        self.responses = []
        for i in range(n):
            response = data.readline().strip()
            if len(response) != n:
                raise ValueError('response must have length n')
            if not re.fullmatch('[01]+', response):
                raise ValueError('response must contain only zeros and ones')
            if state[i] == '1' and response != state:
                raise ValueError('working machines must report correct answers')
            self.responses.append(response)


def load_batch(batch_idx: int, data: TextIO) -> Batch:
    """Load a batch from file and report the batch number on error."""
    try:
        return Batch(data)
    except Exception as exc:
        raise RuntimeError('Error loading batch {} from file'.format(batch_idx + 1)) from exc


def interact(process: subprocess.Popen, batch: Batch, *, verbose: bool) -> int:
    try:
        vprint(batch.n, file=process.stdin, verbose=verbose)
        queries = 0
        while True:
            line = vreadline(process.stdout, verbose=verbose)
            if line == '':
                raise WrongAnswer('End of file received from team program')
            tokens = line.split()
            if not tokens:
                raise WrongAnswer('Empty line received')
            command = tokens[0]
            tokens = tokens[1:]
            if command.lower() == 'test':
                if queries == MAX_QUERIES:
                    raise WrongAnswer('Too many queries')
                r = len(tokens)
                if r != batch.n:
                    raise WrongAnswer(f'Expected {batch.n} machines to test, received {r}')
                try:
                    machines = [int(x) for x in tokens]
                except ValueError:
                    raise WrongAnswer('Machine ID is not an integer')
                if min(machines) < 0 or max(machines) > batch.n:
                    raise WrongAnswer('Machine ID is out of range')
                response = []
                seen = set()
                for i, machine in enumerate(machines):
                    if machine == 0:
                        response.append('-')
                    elif machine in seen:
                        raise WrongAnswer('Machine tested twice in one query')
                    else:
                        seen.add(machine)
                        response.append(batch.responses[i][machine - 1])
                vprint(''.join(response), file=process.stdin, verbose=verbose)
                queries += 1
            elif command.lower() == 'answer':
                r = len(tokens)
                if r != 1:
                    raise WrongAnswer(f'Expected one token for answer, received {r}')
                if tokens[0] != batch.state:
                    raise WrongAnswer(f'Incorrect answer {tokens[0]}, expected {batch.state}')
                break
            else:
                raise WrongAnswer(f'Unrecognised command {command!r}')
    except BrokenPipeError:
        raise WrongAnswer('Error when sending response to team program - possibly exited')
    return queries


def main() -> int:
    parser = argparse.ArgumentParser(usage='%(prog)s [--verbose] data.in program [args...]')
    parser.add_argument('--verbose', '-v', action='store_true', help='Show interactions')
    parser.add_argument('data')
    parser.add_argument('program', nargs=argparse.REMAINDER)

    args = parser.parse_args()
    if not args.program:
        parser.error('Must specify program to run')

    with open(args.data, 'r') as data:
        b = int(data.readline())
        batches = []
        for batch_idx in range(b):
            batches.append(load_batch(batch_idx, data))

    max_queries = 0
    process = subprocess.Popen(args.program, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               encoding='utf-8', errors='surrogateescape')
    try:
        vprint(b, file=process.stdin, verbose=args.verbose)
        for batch_idx, batch in enumerate(batches):
            try:
                queries = interact(process, batch, verbose=args.verbose)
            except WrongAnswer as exc:
                print('Wrong answer (on batch {}: {})'.format(batch_idx + 1, exc))
                return 1
            else:
                print('Batch {} correctly answered in {} queries'.format(batch_idx + 1, queries))
                max_queries = max(max_queries, queries)
        process.wait()
    finally:
        if process.poll() is None:
            try:
                process.terminate()
            except ProcessLookupError:  # Should be impossible, but just to be safe
                pass
        process.wait()
    if process.returncode < 0:
        print(f'Run-time error (process exited with signal {-process.returncode})')
        return 1
    elif process.returncode > 0:
        print(f'Run-time error (process exited with status {process.returncode})')
        return 1
    else:
        print(f'Accepted (max queries = {max_queries})')
        return 0


if __name__ == '__main__':
    sys.exit(main())
