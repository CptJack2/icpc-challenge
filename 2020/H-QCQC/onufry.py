#!/usr/bin/env python3
from __future__ import print_function, division
import sys

testcnt = int(sys.stdin.readline())
for testnum in range(testcnt):
  N = int(sys.stdin.readline())

  def real_query(targets):
    res = "test "
    for i, t in enumerate(targets):
      if i == t:
        res += "0 "
      else:
        res += str(t+1)
        res += " "
    print(res)
    sys.stdout.flush()
    resp = sys.stdin.readline()
    results = []
    for i, t in enumerate(targets):
      results.append((resp[i] == "1"))
    return results
  
  def output(good, bad):
    res = "answer "
    for i in range(N):
      if i in good:
        res += "1"
      else:
        assert i in bad
        res += "0"
    print(res)
  
  # Queries is a list of pairs of machines that query one another.
  # The result is a dict from machine number to the result of the query
  #   (True or False, where False indicates one of them is wrong).
  def query(queries):
    actual = list(range(N))
    for a, b in queries:
      actual[a] = b
      actual[b] = a
    actual_result = real_query(actual)
    result = {}
    for a, b in queries:
      if actual_result[a] and actual_result[b]:
        result[a] = True
        result[b] = True
      else:
        result[a] = False
        result[b] = False
    return result
  
  # Active and leftovers are lists of tuples.
  # Paired is a list of pairs of tuples.
  def phase_one_step(active, leftovers, paired):
    queries = []
    for i in range(len(active) // 2):
      queries.append((active[2 * i][0], active[2 * i + 1][0]))
    if len(active) % 2:
      for machine, lefto in zip(active[-1], leftovers):
        queries.append((machine, lefto[0]))
    result = query(queries)
    new_active = []
    for i in range(len(active) // 2):
      if result[active[2 * i][0]]:
        new_active.append(active[2*i] + active[2*i + 1])
      else:
        paired.append((active[2*i], active[2*i + 1]))
    new_leftovers = []
    if len(active) % 2:
      for lefto in leftovers:
        if result[lefto[0]]:
          active[-1] += lefto
        else:
          new_leftovers.append(lefto)
      new_leftovers.append(active[-1])
    else:
      new_leftovers = leftovers
    return (new_active, new_leftovers, paired)
  
  def intermediate(active, leftovers, paired):
    queries = []
    good = []
    bad = []
    pairs = []
    if len(active) == 0:
      good += leftovers[-1]
      for lefto in leftovers[:-1]:
        bad += lefto
      queries.append((good[0], paired[-1][0][0]))
      result = query(queries)
      if result[good[0]]:
        good += paired[-1][0]
        bad += paired[-1][1]
      else:
        good += paired[-1][1]
        bad += paired[-1][0]
      for pai in paired[:-1]:
        for a, b in zip(pai[0], pai[1]):
          pairs.append((a,b))
    else:
      assert len(active) == 1 
      good += active[0]
      for pai in paired:
        for a, b in zip(pai[0], pai[1]):
          pairs.append((a, b))
      if leftovers:
        for lefto, goo in zip(leftovers, good):
          queries.append((lefto[0], goo))
        result = query(queries)
        for lefto, _ in zip(leftovers, good):
          if result[lefto[0]]:
            good += lefto
          else:
            bad += lefto
    return good, bad, pairs
  
  def final_step(good, bad, pairs):
    queries = []
    new_pairs = []
    if len(good) - len(bad) < 3:
      for g, p in zip(good, pairs):
        queries.append((g, p[0]))
      res = query(queries)
      for i, p in enumerate(pairs):
        if i >= len(good):
          new_pairs.append(p) 
      for g, p in list(zip(good, pairs)):
        if res[g]:
          good.append(p[0])
          bad.append(p[1])
        else:
          good.append(p[1])
          bad.append(p[0])
    else:
      goodpairs = []
      for i in range(len(good) // 2):
        goodpairs.append((good[2 * i], good[2 * i + 1]))
      for g, p in zip(goodpairs, pairs):
        queries.append((g[0], p[0]))
        queries.append((g[1], p[1]))
      res = query(queries)
      for i, p in enumerate(pairs):
        if i >= len(goodpairs):
          new_pairs.append(p)
      for (g, p) in zip(goodpairs, pairs):
        if res[g[0]]:
          good.append(p[0])
        else:
          bad.append(p[0])
        if res[g[1]]:
          good.append(p[1])
        else:
          bad.append(p[1])
    return good, bad, new_pairs
  
  # MAIN
  active = [(i,) for i in range(N)]
  leftover = []
  paired = []
  
  while len(active) > 1:
    active, leftover, paired = phase_one_step(active, leftover, paired)
  good, bad, pairs = intermediate(active, leftover, paired)
  while len(pairs):
    good, bad, pairs = final_step(good, bad, pairs)
  output(good, bad)

  sys.stdout.flush()
