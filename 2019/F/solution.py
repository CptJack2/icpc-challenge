# -*- coding:utf-8 -*-
import bisect
from operator import attrgetter

def get_dep(N,allX,data):
    #各个tarp之间的上下依赖关系
    dependency=[set() for i in range(N+2)]
    #从左到右扫描辅助数据
    class scan_set_stru:
        def __init__(self, index, height):
            self.index=index
            self.height=height

    #沿着x轴从左到右扫描
    scan_set=[allX[0].index]
    start=allX[1]
    #allX[0]-allX[1]的扫描结果就是上面两行的赋值
    #循环里处理[start.x,end.x)这个区间
    for allXList in allX:
        current_x=allXList[0].x
        #先将在a点要退出的tarp全部清理出去
        #先过滤掉区间左端点，类型为out且端点为低端的，这类上面的水直接越过端点滴到下面
        for allx_stru in allXList:
            data_index=allx_stru.index
            x1,y1,x2,y2=data[data_index]
            if y1<y2:
                x1,y1,x2,y2=x2,y2,x1,y1
            if x1==current_x: low_out=True
            else:low_out=False
            if allx_stru.type=="out" and low_out:
                scan_set_index=scan_set.index(data_index)
                allXList.remove(allx_stru)
                dep_index=scan_set_index
                def get_under_line_in_group(x,li,group):
                    possible=[]
                    for lgi in group:
                        possible.append(lgi)
                        x1,y1,x2,y2=data[lgi]
                while dep_index>=0 and
                    dependency[allx_stru.index].append()
        #剩下的tarp都是连续存在与区间[a,b)之内,水只能从上面的tarp滴到下面的tarp
        for scan_set_i in range(len(scan_set)-1):
            dependency[scan_set_i+1].add(scan_set_i)
        #处理到end_x+1是因为要在这轮循环把tarp移入/出scan_set
        y_by_x=[cal_y(x,*data[scan_set_i])]
        if start.type=="in":
            h=cal_y(start.x,*data[start.index])
            for insert_pos in range(len(scan_set)):
                if scan_set[insert_pos].height>h:break
            scan_set.insert(insert_pos,start.index)
        else:
            scan_set.remove(end.index)
        for x in range(max(x1,start.x+1),min(x2,end.x+1)):
            #tarp移入/出scan_set
            if x==end.x:
                if end.type=="in":
                    h=cal_y(end,*data[end.index])
                    for insert_pos in range(len(scan_set)):
                        if scan_set[insert_pos].height>h:break
                    scan_set.insert(insert_pos,end.index)
                    y_by_x.insert(insert_pos,h)
                else:
                    scan_set.remove(end.index)
                #更新各个tarp的依赖
                for scan_set_j in range(0,scan_set_i):
                    x1=min(data[scan_set_j][0],data[scan_set_j][2])
                    x2=max(data[scan_set_j][0],data[scan_set_j][2])
                    for scan_x in range(last_x+1,current_x+1):
                        if scan_x<x1 or scan_x>x2:continue


    while allX_i<len(allX):
        current_x=allX[allX_i].x
        #处理current x坐标下所有的扇入
        while allX_i<len(allX) and allX[allX_i].x==current_x:
            xi=allX[allX_i]
            #到尽头的tarp需要清出
            for scan_stru in scan_set:
                x2=max(data[scan_stru.index][0],data[scan_stru.index][2])
                if x2<=xi.x:
                    #这个tarp的终结位置，它上面tarp的水可以直接滴到它下面的
                    scan_set_ind=scan_set.index(scan_stru)
                    if scan_set_ind>0 and scan_set_ind <len(scan_set)-1:
                        dependency[scan_set[scan_set_ind+1].index].add(scan_set[scan_set_ind-1].index)
                    scan_set.remove(scan_stru)
            #根据在同一个垂直扫描线上的x坐标，计算每条tarp对应的y坐标，根据y坐标进行排序，并用插入排序把新加入的tarp放到正确的位置
            for s in scan_set:
                s.height=cal_y(xi.x,*data[s.index])
            h=cal_y(xi.x,*data[xi.index])
            i=0
            while i<len(scan_set) and scan_set[i].height<h:
                i+=1
            scan_set.insert(i,scan_set_stru(xi.index,h))
            #新插入的tarp，处理它和scan_set中已有tarp的dependency
            if data[xi.index][1]<data[xi.index][3] and  i>0:#这里需要判断i>0是因为这个tarp可能是这条x垂直线上最底下的一个tarp
                dependency[xi.index].add(scan_set[i-1].index)
            #对scan_set中每个tarp, 从上次扫描的终止点扫描到此次扫描的起点
            for scan_i,scan_stru in enumerate(scan_set):
                x1=min(data[scan_stru.index][0],data[scan_stru.index][2])
                x2=max(data[scan_stru.index][0],data[scan_stru.index][2])
                for scan_x in range(last_x+1,current_x+1):
                    if scan_x<x1 or scan_x>x2:continue

                #如果左端点是这个tarp的较低点, 他的水可以直接滴到他下方的tarp上,

                #往下搜寻新tarp依赖的
                for j in range(i,0,-1):
                    x2=max(data[j][0],data[j][2])
                    if x2>current_x+1:

                #往上搜寻依赖新tarp的

                if i<len(scan_set)-1:
                    dependency[scan_set[i+1].index].append(xi.index)
            last_x=current_x
            allX_i+=1
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