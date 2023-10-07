#!/usr/bin/python

import sys

if(sys.argv[1]=="impossible"):
    if(sys.argv[1]==sys.argv[2]):
        print("succ")
    else:
        print("failed")
    exit(0)

f1=float(sys.argv[1])
f2=float(sys.argv[2])

if(abs(f1-f2)>1e-6):
    print("failed")
    print("correct")
    print(f1)
    print("ans")
    print(f2)
else:
    print("succ")