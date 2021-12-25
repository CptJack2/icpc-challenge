c=0
def myinput(n):
    src=[
    10,20,5,
    32, 50, 12, 60,
    30, 60, 8, 70,
    25, 70, 0, 80,
    15, 30, 28, 40,
    5, 20, 14, 25,
    ]
    global c
    ret=src[c:c+n]
    c+=n
    return ret

L,R,N=myinput(3)
print(L,R,N)
data=[(L,0,R,0)]
allX=[(L,0,"in"),(R,0,"out")]
for i in range(1,N+1):
    x1,y1,x2,y2=myinput(4)
    if x1>x2:
        x1,y1,x2,y2=x2,y2,x1,y1
    data.append((x1,y1,x2,y2))
    allX.append((x1,i,"in"))
    allX.append((x2,i,"out"))
allX.sort()

dependency=[[] for i in range(N+1)]
scan_set={}
def cal_y(x,x1,x2,y1,y2):
    return float(y1-y2)/(x1-x2)*(x-x1)+y1
for xi in allX:
    if xi[3]=="in":
        for i in set:
            if dep
