# -*- coding:utf-8 -*-
import pdb
from operator import attrgetter

def get_dep(N,allX,data):
    #各个tarp之间的上下依赖关系
    dependency=[[] for i in range(N+2)]
    #从左到右扫描辅助数据
    scan_set=[]
    class scan_set_stru:
        def __init__(self, index, height):
            self.index=index
            self.height=height

    #沿着x轴从左到右扫描
    for xi in allX:
        if xi.type=="in":
            for s in scan_set:
                s.height=cal_y(xi.x,*data[s.index])
            #根据在同一个垂直扫描线上的x坐标，计算每条tarp对应的y坐标，根据y坐标进行排序，并用插入排序把新加入的tarp放到正确的位置
            h=cal_y(xi.x,*data[xi.index])
            i=0
            #到尽头的tarp需要清出
            for t in scan_set:
                x2=max(data[t.index][0],data[t.index][2])
                if x2<=xi.x:
                    scan_set.remove(t)
            while i<len(scan_set) and scan_set[i].height<h:
                i+=1
            scan_set.insert(i,scan_set_stru(xi.index,h))
            #新插入的tarp，处理它和直接相邻的上下两个tarp的依赖关系
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
            #这个tarp的终结位置，它上面tarp的水可以直接滴到它下面的
            if ind!=0 and ind<len(scan_set):
                dependency[scan_set[ind].index].append(scan_set[ind-1].index)
    return dependency

    #按照tarp的依赖关系拓扑排序
def topo_sort(dep):
    ret=[]
    while len(ret)<len(dep):
        #有没有入度为0的点
        noAllMet=True
        for ind,dl in enumerate(dep):
            if ind in ret:
                continue
            #寻找入度为0的点
            m=True
            for d in dl:
                if d not in ret:
                    m=False
                    break
            if m:
                noAllMet=False
                ret.append(ind)
        #没有入度为0的点，没法排了
        if noAllMet:
            raise Exception("can't topology sort")
    return ret

def get_data_from_input():
    L,R,N=[int(i) for i in input().split()]
    maxy=-1
    #tarp位置数据
    data=[(L,0,R,0)]
    class allX_stru:
        def __init__(self,x,index,type):
            self.x=x
            self.index=index
            self.type=type
    #所有x坐标，排序后从左到右扫描
    allX=[allX_stru(L,0,"in"),allX_stru(R,0,"out")]

    for i in range(1,N+1):
        x1,y1,x2,y2=[int(i) for i in input().split()]
        data.append((x1,y1,x2,y2))
        if y2>maxy:
            maxy=y2
        if x1>x2:
            x1,y1,x2,y2=x2,y2,x1,y1
        allX.append(allX_stru(x1,i,"in"))
        allX.append(allX_stru(x2,i,"out"))

    #天花板上加一个，作为最后的结果输出
    if N==0 and maxy==-1:maxy=1
    data.append((L,maxy+1,R,maxy+1))
    allX.append(allX_stru(L,N+1,"in"))
    allX.append(allX_stru(R,N+1,"out"))
    allX.sort(key=attrgetter('x'))

    return L,R,N,data,allX

#计算x在直线上的y坐标
def cal_y(x,x1,y1,x2,y2):
    return float(y1-y2)/(x1-x2)*(x-x1)+y1

if __name__=='__main__':
    L,R,N,data,allX=get_data_from_input()

    dependency=get_dep(N,allX,data)
    #天花板不参与拓扑排序
    sorted=topo_sort(dependency[:-1])

    inf=99999999999
    #0号地板先特殊处理
    sorted.remove(0)
    dp=[[] for i in range(N+1)]
    dp[0]=[0 for i in range(R-L+1)]

    #获取序号为ind的tarp在x处下方的tarp，没有返回-1
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

    #根据拓扑排序结果
    for ind in sorted:
        xs=data[ind][0]
        xe=data[ind][2]
        #沿着水流的逆向进行动态规划
        step=1 if xs<xe else -1
        uind=get_under_line(xs,ind)
        dp[ind]=[inf for i in range(xs,xe+step,step)]
        #tarp的前端只能往下流
        if uind==-1:
            #下面没有tarp接着，无解
            dp[ind][0]=inf
        else:
            #下面有tarp，取它的解
            uxs=data[uind][0]
            uxe=data[uind][2]
            ustep=1 if uxs<uxe else -1
            #only down without drill
            dp[ind][0]=dp[uind][int((xs-uxs)/ustep)]
        #tarp的其余点，可以选择打洞往下或者沿着tarp流下
        for i,x in enumerate(range(xs+step,xe+step,step)):
            #flow or down
            it=i+1
            uind=get_under_line(x,ind)
            #下面没有tarp只能往前流
            if uind==-1:
                dp[ind][it]=dp[ind][it-1]
            else:
                #下面有tarp则取打洞往下和往前的较小代价
                uxs=data[uind][0]
                uxe=data[uind][2]
                ustep=1 if uxs<uxe else -1
                #dp数组的序号0-n对应tarp沿水流逆向的起点到终点
                dp[ind][it]=min(dp[uind][int((x-uxs)/ustep)]+1,dp[ind][it-1])

    #最后从天花板找最小值就可以了
    ans=9999999
    for i in dependency[N+1]:
        for j in dp[i]:
            if j<ans:
                ans=j
    print(ans,"\n")