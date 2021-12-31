# -*- coding:utf-8 -*-
import bisect
from operator import attrgetter

def get_dep(N,allX,data):
    #各个tarp之间的上下依赖关系
    dependency=[set() for i in range(N+2)]
    #边界点的类型
    xtype_continuous=1;xtype_lowin=2;xtype_lowout=3;xtype_highin=4;xtype_highout=5;
    def get_xtype(current_x,data_index):
        x1, y1, x2, y2 = data[data_index]
        if y1 < y2:
            x1, y1, x2, y2 = x2, y2, x1, y1
        if x1 == current_x:
            if x1 < x2:
                xtype = xtype_lowin
            else:
                xtype = xtype_lowout
        elif x2 == current_x:
            if x1 < x2:
                xtype = xtype_highout
            else:
                xtype = xtype_highin
        else:
            xtype = xtype_continuous
        return xtype

    #从左到右扫描辅助数据
    class scan_set_stru:
        def __init__(self, index,type,height):
            self.index=index
            self.type=type
            self.height=height
    scan_set=[]
    for xstru in allX[0]:
        current_x=allX[0][0].x
        scan_set.append(scan_set_stru(xstru.index,get_xtype(current_x,xstru.index)))
    interval_left=allX[0][0].x
    #沿着x轴从左到右扫描,循环里处理[start.x,end.x)这个区间
    for allXList in allX[1:]:
        interval_right=allXList[0].x
        #加入in类型的tarp
        for allx_stru in allXList:
            data_index=allx_stru.index
            xtype=get_xtype(interval_left,data_index)
            if xtype==xtype_lowin or xtype==xtype_highin:
                scan_set.append(scan_set_stru(data_index,xtype))
        #模拟水从上层滴落到下层计算dep
        for i in range(len(scan_set)-1,-1,-1):
            tup=scan_set[i]
            for j in range(i-1,-1,-1):
                tdown=scan_set[j]
                if tdown.type!=xtype_lowin and tdown.type!=xtype_lowout:
                    dependency[tdown.index].add(tup.index)
                if tdown.type==xtype_continuous:
                    break
        #清退out类型的tarp
        for allx_stru in allXList:
            data_index=allx_stru.index
            xtype=get_xtype(interval_left,data_index)
            if xtype==xtype_lowout or xtype==xtype_highout:
                scan_set.remove(scan_set_stru(data_index,xtype))

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
    def insertAllX(new_stru):
        new_stru=allX_stru(new_stru)
        insert_pos=0
        for insert_pos in range(len(allX)):
            if allX[insert_pos][0].x>=new_stru.x:break
        if allX[insert_pos][0].x==new_stru.x:
            allX[insert_pos].append(new_stru)
        else:
            allX.insert(insert_pos,[new_stru])
    #allX是一个二维数组，每一个子数组是allX_stru的数组，存放在这个x坐标下端点的信息
    allX=[]
    insertAllX(allX_stru(L,0,"in"))
    insertAllX(allX_stru(R,0,"out"))

    for i in range(1,N+1):
        x1,y1,x2,y2=[int(i) for i in input().split()]
        data.append((x1,y1,x2,y2))
        if y2>maxy:
            maxy=y2
        insertAllX(allX_stru(min(x1,x2),i,"in"))
        insertAllX(allX_stru(max(x1,x2),i,"out"))

    #天花板上加一个，作为最后的结果输出
    if N==0 and maxy==-1:maxy=1
    data.append((L,maxy+1,R,maxy+1))
    insertAllX(allX_stru(L,N+1,"in"))
    insertAllX(allX_stru(R,N+1,"out"))

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