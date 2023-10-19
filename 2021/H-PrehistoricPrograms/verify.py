#!/usr/bin/python

import subprocess
import sys

fn=sys.argv[1]

def execute_bash_command(command):
    result = subprocess.run("bash -c \""+command+"\"", shell=True, capture_output=True, text=True)
    if result.returncode == 0:
        output = result.stdout
        return output

out=execute_bash_command("cat data/"+fn+".in | ./main").strip()
if out=="impossible":
    ans=execute_bash_command("cat data/"+fn+".ans").strip()
    if ans==out:
        print("succ")
    else:
        print("failed")
    exit()

strs=[]
with open("data/"+fn+".in") as f:
    n=int(f.readline())
    for i in range(n):
        strs.append(f.readline().strip())

indexs = [int(x) for x in out.split("\n") if x]
str=""
for i in indexs:
    if i-1>=len(strs):
        print("failed")
        exit()
    str+=strs[i-1]
stack=[]
for c in str:
    if c=='(':
        stack.append(0)
    else:
        if len(stack)==0:
            print("failed")
            exit()
        stack.pop()
if len(stack)==0:
    print("succ")
else:
    print("failed")