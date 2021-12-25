from operator import attrgetter

c=0
def myinput(n):
    # src=[
    # 10,20,5,
    # 32, 50, 12, 60,
    # 30, 60, 8, 70,
    # 25, 70, 0, 80,
    # 15, 30, 28, 40,
    # 5, 20, 14, 25,
    # ]
    src=[
    2,4,2,
    3,2,0,3,
    5,2,1,5,
    ]
    global c
    ret=src[c:c+n]
    c+=n
    return ret

L,R,N=myinput(3)
maxy=-1
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
    data.append((x1,y1,x2,y2))
    if y2>maxy:
        maxy=y2
    if x1>x2:
        x1,y1,x2,y2=x2,y2,x1,y1
    allX.append(allX_stru(x1,i,"in"))
    allX.append(allX_stru(x2,i,"out"))

data.append((L,maxy+1,R,maxy+1))
allX.append(allX_stru(L,N+1,"in"))
allX.append(allX_stru(R,N+1,"out"))
allX.sort(key=attrgetter('x'))

dependency=[[] for i in range(N+2)]
scan_set=[]
class scan_set_stru:
    def __init__(self, index, height):
        self.index=index
        self.height=height

def cal_y(x,x1,y1,x2,y2):
    return float(y1-y2)/(x1-x2)*(x-x1)+y1
for xi in allX:
    if xi.type=="in":
        for s in scan_set:
            s.height=cal_y(xi.x,*data[s.index])
        h=cal_y(xi.x,*data[xi.index])
        i=0
        while i<len(scan_set) and scan_set[i].height<h:
            i+=1
        scan_set.insert(i,scan_set_stru(xi.index,h))
        if i>0:
            dependency[xi.index].append(scan_set[i-1].index)
        if i<len(scan_set)-1:
            dependency[scan_set[i+1].index].append(xi.index)
    else:
        ind=-1
        for i,s in enumerate(scan_set):
            if s.index==xi.index:
                ind=i
                break
        scan_set.pop(ind)
        if ind!=0 and ind<len(scan_set):
            dependency[scan_set[ind].index].append(scan_set[ind-1].index)

def topo_sort(dep):
    ret=[]
    while len(ret)<len(dep):
        noAllMet=True
        for ind,dl in enumerate(dep):
            if ind in ret:
                continue
            m=True
            for d in dl:
                if d not in ret:
                    m=False
                    break
            if m:
                noAllMet=False
                ret.append(ind)
        if noAllMet:
            raise Exception("can't topology sort")
    return ret

sorted=topo_sort(dependency)

inf=99999999999
minx=allX[0].x
maxx=allX[-1].x
sorted.remove(0)
dp=[[] for i in range(N+2)]
dp[0]=[0 for i in range(R-L+1)]

def get_under_line(x,ind):
    dep=dependency[ind]
    h=-1;i=-1
    mh=cal_y(x,*data[ind])
    for d in dep:
        x1,x2=data[d][0],data[d][2]
        if x1>x2:
            x1,x2=x2,x1
        if not (x1<=x<=x2):
            continue
        th=cal_y(x,*data[d])
        if th>h and th<mh:
            i=d
            h=th
    return i

for ind in sorted:
    xs=data[ind][0]
    xe=data[ind][2]
    step=1 if xs<xe else -1
    uind=get_under_line(xs,ind)
    dp[ind]=[inf for i in range(xs,xe+step,step)]
    if uind==-1:
        dp[ind][0]=inf
    else:
        uxs=data[uind][0]
        uxe=data[uind][2]
        ustep=1 if uxs<uxe else -1
        #only down without drill
        dp[ind][0]=dp[uind][int((xs-uxs)/ustep)]
    for i,x in enumerate(range(xs+step,xe+step,step)):
        #flow or down
        it=i+1
        uind=get_under_line(x,ind)
        if uind==-1:
            dp[ind][it]=dp[ind][it-1]
        else:
            uxs=data[uind][0]
            uxe=data[uind][2]
            ustep=1 if uxs<uxe else -1
            dp[ind][it]=min(dp[uind][int((x-uxs)/ustep)]+1,dp[ind][it-1])

min=9999999
for i in dp[-1]:
    if i<min:
        min=i
print(i,"\n")