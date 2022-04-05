#!/usr/bin/python

import sys

f1=float(sys.argv[1])
f2=float(sys.argv[2])

if(abs(f1-f2)>1e-3):
    print("failed")
else:
    print("succ")