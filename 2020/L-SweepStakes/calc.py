#!/usr/bin/python
import pdb
import sys

fileName=str(sys.argv[1])

row=0
with open('data/my_ans') as fans:
    with open('data/'+fileName+'.ans') as fcor:
        for al in fans:
            row+=1
            a=[float(x) for x in al.split()]
            b=[float(x) for x in fcor.readline().split()]
            col=0
            for i,j in zip(a,b):
                col+=1
                if(abs(i-j)>1e-6):
                    # print("failed, row: ",row," col: ",col)
                    # print("ans: ",i)
                    # print("correct: ",j)
                    print("failed")
                    exit(0)
print("succ")