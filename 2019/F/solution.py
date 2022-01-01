# -*- coding:utf-8 -*-
import bisect
from operator import attrgetter

def get_dep(N,allX,data):
    #各个tarp之间的上下依赖关系
    dependency=[set() for i in range(N+2)]
    #从左到右扫描,加入集合
    scan_set=[]
    interval_left=allX[0][0].x
    def add_dep(i,j):
        dependency[i].add(j)
    #沿着x轴从左到右扫描,循环里处理[start.x,end.x)这个区间
    for i in range(1,len(allX)):
        allXList=allX[i-1]
        interval_right=allX[i][0].x
        in_tarp= {}
        out_tarp={}
        #加入in类型的tarp
        for allx_stru in allXList:
            data_index=allx_stru.index
            if allx_stru.direction=="in":
                ys=[cal_y(interval_left,*data[s]) for s in scan_set]
                h=cal_y(interval_left,*data[data_index])
                insert_pos=0
                for insert_pos in range(len(ys)):
                    if ys[insert_pos]>h:break
                if insert_pos==len(ys)-1 and h>ys[-1]:
                    scan_set.append(data_index)
                else:
                    scan_set.insert(insert_pos,data_index)
                in_tarp[data_index]=allx_stru
            else:
                out_tarp[data_index]=allx_stru

        #对于新加入的tarp,模拟水从上层滴落到下层计算dep
        for tnew in in_tarp:
            tnew_scan_index=scan_set.index(tnew)
            #向上处理需要依赖新tarp的, low out的tarp无需被依赖
            if not (tnew in out_tarp and out_tarp[tnew].slant!="low"):
                only_left=False
                for j in range(tnew_scan_index+1,len(scan_set)):
                    tup=scan_set[j]
                    if j==tnew_scan_index+1 or in_tarp[tnew].slant!="low":
                        add_dep(tup,tnew)
                    #如果上面的tarp在[a,b)出, 不能阻挡水往下
                    if tup in out_tarp:
                        continue
                    #如果上面的tarp在[a,b)进, 水只能在a点往下
                    if not only_left and tup in in_tarp:
                        only_left=True
                        continue
                    break
            #向下处理新tarp依赖的
            for j in range(tnew_scan_index-1,-1,-1):
                tdown=scan_set[j]
                #直接下方的tarp、有一个端点在a上，可以直接穿过，但不是低点、直接横穿[a,b)的tarp
                if j==tnew_scan_index-1 or \
                    (tdown in out_tarp and out_tarp[tdown].slant!="low") or\
                    (tdown in in_tarp and in_tarp[tdown].slant!="low") or \
                    (tdown not in out_tarp and tdown not in in_tarp) :
                    add_dep(tnew,tdown)
                #如果下面的tarp在[a,b)出, 不能阻挡水往下
                if tdown in out_tarp or tdown in in_tarp:
                    continue
                break
        #tarp移出的时候, 它挡住的位置向上可达和向下可达的tarp, 有依赖关系
        for tout in out_tarp:
            tout_scan_index=scan_set.index(tout)
            up_reachable=set()
            only_left=False
            for j in range(tout_scan_index+1,len(scan_set)):
                tup=scan_set[j]
                up_reachable.add(tup)
                #如果上面的tarp在[a,b)出, 不能阻挡水往下
                if tup in out_tarp:
                    continue
                #如果上面的tarp在[a,b)进, 水只能在a点往下
                if not only_left and tup in in_tarp:
                    only_left=True
                    continue
                break
            down_reachable=set()
            for j in range(tout_scan_index-1,-1,-1):
                tdown=scan_set[j]
                #todo 确认是不是跟上面一样
                if j==tout_scan_index-1 or\
                    (tdown in out_tarp and out_tarp[tdown].slant!="low") or \
                    (tdown in in_tarp and in_tarp[tdown].slant!="low") or \
                    (tdown not in out_tarp and tdown not in in_tarp):
                    down_reachable.add(tdown)
                #如果下面的tarp在[a,b)出, 不能阻挡水往下
                if tdown in out_tarp or tdown in in_tarp:
                    continue
                break
            #出tarp本身也可以在这个点被依赖
            if out_tarp[tout].slant!="low":
                down_reachable.add(tout)
            for a in up_reachable:
                for b in down_reachable:
                    add_dep(a,b)
        #清退out类型的tarp
        for allx_stru in allXList:
            data_index=allx_stru.index
            if allx_stru.direction=="out":
                scan_set.remove(data_index)
        interval_left=interval_right
    #处理区间最右端点,这时候只剩下out类型的点了
    # for i in allX[-1]:
    #     for j in allX[-1]:
    #         if i.index==j.index:continue
    #         if i.slant!="low":
    #             add_dep(j.index,i.index)

    for i in range(len(scan_set)-1,-1,-1):
        tup=scan_set[i]
        for j in range(i-1,-1,-1):
            for str in allX[-1]:
                if str.index==scan_set[j]:break
            if str.slant!="low":
                add_dep(tup,str.index)

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
        def __init__(self,x,index,slant,direction):
            self.x=x
            self.index=index
            self.slant=slant
            self.direction=direction
    def insertAllX(new_stru):
        insert_pos=0
        for insert_pos in range(len(allX)):
            if allX[insert_pos][0].x>=new_stru.x:break
        if insert_pos<len(allX) and allX[insert_pos][0].x==new_stru.x:
            allX[insert_pos].append(new_stru)
        elif insert_pos==len(allX)-1 and allX[-1][0].x< new_stru.x:
            allX.append([new_stru])
        else:
            allX.insert(insert_pos,[new_stru])
    #allX是一个二维数组，每一个子数组是allX_stru的数组，存放在这个x坐标下端点的信息
    allX=[]
    insertAllX(allX_stru(L,0,"parallel","in"))
    insertAllX(allX_stru(R,0,"parallel","out"))

    for i in range(1,N+1):
        x1,y1,x2,y2=[int(i) for i in input().split()]
        data.append((x1,y1,x2,y2))
        if y2>maxy:
            maxy=y2
        if x1>x2:
            x1,y1,x2,y2=x2,y2,x1,y1
        if y1<y2:
            left="low"
            right="high"
        else:
            left="high"
            right="low"
        insertAllX(allX_stru(x1,i,left,"in"))
        insertAllX(allX_stru(x2,i,right,"out"))

    #天花板上加一个，作为最后的结果输出
    if N==0 and maxy==-1:maxy=1
    data.append((L,maxy+1,R,maxy+1))
    insertAllX(allX_stru(L,N+1,"parallel","in"))
    insertAllX(allX_stru(R,N+1,"parallel","out"))

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