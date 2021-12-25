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
class allX_stru:
    def __init__(self,x,index,type):
        self.x=x
        self.index=index
        self.type=type
allX=[allX_stru(L,0,"in"),allX_stru(R,0,"out")]

for i in range(1,N+1):
    x1,y1,x2,y2=myinput(4)
    if x1>x2:
        x1,y1,x2,y2=x2,y2,x1,y1
    data.append((x1,y1,x2,y2))
    allX.append(allX_stru(x1,i,"in"))
    allX.append(allX_stru(x2,i,"out"))
allX.sort()

dependency=[[] for i in range(N+1)]
scan_set=[]
class scan_set_stru:
    def __init__(self, index, height):
        self.index=index
        self.height=height

def cal_y(x,x1,x2,y1,y2):
    return float(y1-y2)/(x1-x2)*(x-x1)+y1
for xi in allX:
    if xi.type=="in":
        for s in scan_set:
            s.height=cal_y(xi.x,*data[s.index])
        h=cal_y(xi.x,*data[xi.index])
        i=0
        while i<len(scan_set) and scan_set[i].height<h:
            i+=1
        scan_set.insert(i,scan_set_stru(xi.x,h))
        if i>0:
            dependency[xi.index].append(scan_set[i-1].index)
        if i<len(scan_set)-1:
            dependency[scan_set[i+1].index].append(xi.index)
    else:
        ind=scan_set.index(xi.index)
        scan_set.remove(xi.index)
        if ind!=0:
            dependency[scan_set[ind].index].append(scan_set[ind-1].index)
print("hello")
