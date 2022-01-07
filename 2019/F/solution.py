# -*- coding:utf-8 -*-

def get_dep(N,allX,data):
    #各个tarp之间的上下依赖关系, 内层数组元素是tuple，0为依赖发生的x坐标，1为依赖的index
    dependency=[[] for i in range(N+1)]
    #从左到右扫描,加入集合
    scan_set=[]
    def add_dep(i,j):
        dependency[i].append(j)

    #沿着x轴从左到右扫描,循环里处理[a,b)这个区间
    for xstru in allX:
        if xstru.direction=="in":
            ys=[cal_y(xstru.x,*data[s]) for s in scan_set]
            h=cal_y(xstru.x,*data[xstru.index])
            insert_pos=0
            for insert_pos in range(len(ys)):
                if ys[insert_pos]>h:break
            if insert_pos==len(ys)-1 and h>ys[-1]:
                scan_set.append(xstru.index)
                if len(scan_set)>=2:
                    add_dep(xstru.index,scan_set[-2])
            else:
                scan_set.insert(insert_pos,xstru.index)
                if insert_pos+1<len(scan_set):
                    add_dep(scan_set[insert_pos+1],xstru.index)
                if insert_pos>0:
                    add_dep(xstru.index,scan_set[insert_pos-1])
        else:
            scan_set_index=scan_set.index(xstru.index)
            if scan_set_index>0 and scan_set_index<len(scan_set)-1:
                add_dep(scan_set[scan_set_index+1],scan_set[scan_set_index-1])
            scan_set.pop(scan_set_index)

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
    #tarp位置数据
    data=[(L,0,R,0)]
    class allX_stru:
        def __init__(self,x,index,direction):
            self.x=x
            self.index=index
            self.direction=direction
    def insertAllX(new_stru):
        allX.append(new_stru)
    #allX是一个二维数组，每一个子数组是allX_stru的数组，存放在这个x坐标下端点的信息
    allX=[]
    insertAllX(allX_stru(L,0,"in"))
    insertAllX(allX_stru(R,0,"out"))

    for i in range(1,N+1):
        x1,y1,x2,y2=[int(i) for i in input().split()]
        data.append((x1,y1,x2,y2))
        if x1>x2:
            x1,x2=x2,x1
        insertAllX(allX_stru(x1,i,"in"))
        insertAllX(allX_stru(x2,i,"out"))

    allX.sort(key=lambda st:st.x)

    return L,R,N,data,allX

#计算x在直线上的y坐标
def cal_y(x,x1,y1,x2,y2):
    return float(y1-y2)/(x1-x2)*(x-x1)+y1

def find_solution_dp():
    minx,maxx=allX[0].x,allX[-1].x
    inf=999999999999
    dp=[inf for i in range(minx,maxx+1)]
    dp[L:R+1]=[0 for i in range(L,R+1)]
    for tarp_index in reversed(sorted):
        #y1<y2
        x1,x2=data[tarp_index][0],data[tarp_index][2]
        step=1 if x1>x2 else -1
        for i in range(x2,x1,step):
            if dp[i+step]>dp[i]:
                dp[i+step]=dp[i]
            if dp[i]!=inf:
                dp[i]+=1
        # for i in range(x2,x1-step,step):
        #     dp[i]+=1

    return min(dp[L:R+1])

if __name__=='__main__':
    L,R,N,data,allX=get_data_from_input()

    dependency=get_dep(N,allX,data)
    sorted=topo_sort(dependency)

    ans=find_solution_dp()

    print(ans,"\n")