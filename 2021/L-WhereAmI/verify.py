#!/usr/bin/python3
import subprocess
import sys

def execute_bash_command(command):
    result = subprocess.run("bash -c \""+command+"\"", shell=True, capture_output=True, text=True)
    if result.returncode == 0:
        output = result.stdout
        return output


if len(sys.argv)>=2:
    fn=sys.argv[1]
else:
    fn="sample-1"

ans=execute_bash_command("cat data/"+fn+".in | ./main")
correct=execute_bash_command("cat data/"+fn+".ans")

f1=float(correct.splitlines()[0])
f2=float(ans.splitlines()[0])

def removeFirstLine(str):
    lines = str.splitlines()[1:]
    result = '\n'.join(lines)
    return result

if(abs(f1-f2)>1e-6):
    print("failed")
    print("correct")
    print(f1)
    print("ans")
    print(f2)
    exit()
if removeFirstLine(correct)!=removeFirstLine(ans):
    print("failed")
    exit()

print("succ")