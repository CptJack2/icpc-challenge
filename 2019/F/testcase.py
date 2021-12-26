from solution import *

case1='''
10 20 5
32 50 12 60
30 60 8 70
25 70 0 80
15 30 28 40
5 20 14 25
'''

case2='''
2 4 2
3 2 0 3
5 2 1 5
'''

case3='''

'''

c=0
src=""
def get_my_input(case):
    global src
    global c
    src=case.split()
    c=0
    return myinput

def myinput(n):
    global src
    global c
    ret=src[c:c+n]
    ret=[int(i) for i in ret]
    c+=n
    return ret

cases=[case1,case2]
for ca in cases:
    ret=solve(get_my_input(ca))
    print("hello")