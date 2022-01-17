#include "bits/stdc++.h"
using namespace std;

int inf=1e8;
class Tarp{
public:
    int X1,X2,Y1,Y2;
    Tarp(int x1,int y1,int x2,int y2):X1(x1),Y1(y1),X2(x2),Y2(y2){}
    Tarp(){}
};
enum direction_enum{
    in=1,
    out=2,
};
struct allx_stru{
    int x,index;
    direction_enum direction;
    allx_stru(int x,int index,enum direction_enum dir): x(x), direction(dir), index(index){}
};
class dp_solver{
public:
	//X cord->delta
	map<int,int>deltas;
	//存储delta为正和负的位置的加速缓存
    set<int> positive_delta_x;
    set<int> negative_delta_x;
    int dp_L_actual,x_L,x_R;
    //插入deltas内没有的新值
    void insert_delta(int x,int delta){
    	deltas[x]=delta;
    	if(delta>0)
    		positive_delta_x.insert(x);
		else
			negative_delta_x.insert(x);
    }
    dp_solver(int L,int R): x_L(L), x_R(R),dp_L_actual(0){
		insert_delta(L,-inf);
		insert_delta(R,inf);
    }
    //对deltas内已有值进行加法操作
    void add_delta(map<int,int>::iterator where,int num){
        int old_delta=where->second;
        auto& old_delta_set= old_delta > 0 ? positive_delta_x : negative_delta_x;
        auto& new_delta_set= old_delta < 0 ? positive_delta_x : negative_delta_x;
        where->second+=num;
        //delta修改后为0，需要移出
        if(where->second==0){
			old_delta_set.erase(where->first);
			deltas.erase(where);
		} else if((old_delta ^ where->second) < 0){//判断delta修改后是否异号. int二进制,负数最高位是1,正数0。负数^正数最高位为1
			//delta变号更新缓存
			new_delta_set.insert(where->first);
            old_delta_set.erase(where->first);
        }
    }
    //滚动区间[start,end]，将前面高的抹平
    void roll(int start, int end){
        int interval_start=start;
    	if(interval_start < end){//从横坐标小往大滚动
            //找出 > start的第一个正delta的x坐标
    		auto it= positive_delta_x.lower_bound(interval_start + 1);
            if(it == positive_delta_x.end())return;
            interval_start=*it;
            while(interval_start <= end){//每个循环内滚动区间[interval_start,interval_end]
                int interval_end;
                //从缓存转到实际的delta区间的位置关系
                auto delta_it=deltas.find(*it);
                //将[interval_start,interval_end]的值用interval_start的值推平，要将interval_start的delta加到下一个区间的delta上
                if (next(delta_it) != deltas.end() && next(delta_it)->first <= end + 1) {
                    interval_end = next(delta_it)->first - 1;//add_delta可能移出为0的delta,导致区间end取错,所以要先取
                    add_delta(next(delta_it),delta_it->second);
                } else {
                	//如果下一个delta区间已经在滚动范围end之外了，或者从end往后都是平的，直接在end+1上插进去一个新delta
                    interval_end = end;
                    insert_delta(end+1,delta_it->second);
                }
                //如果滚动的区间包含了L，要更新一下它的真实值
                if (interval_start <= x_L && interval_end >= x_L)
                    dp_L_actual -= delta_it->second;
                //这个正值的delta被推平了，置为0，从deltas清除
                auto tit = next(it);
                positive_delta_x.erase(it);
                deltas.erase(delta_it);
                if(tit==positive_delta_x.end())break;
                it = tit;
                interval_start=*it;
            }
        } else{//从横坐标大往小滚动，跟上面对称
            ////找出 <= start的第一个负delta的x坐标
    		auto it=negative_delta_x.upper_bound(interval_start);
            if(it == negative_delta_x.begin())return;
            --it;
			interval_start=*it;
            while(interval_start > end){
				int interval_end;
				auto delta_it=deltas.find(*it);
				//将[interval_start,interval_end]的值用interval_start推平，要将interval_start的delta加到下一个区间的delta上
                if (delta_it != deltas.begin() && prev(delta_it)->first >= end) {
                    //更新前一区间delta
                    interval_end = prev(delta_it)->first;
                    add_delta(prev(delta_it),delta_it->second);
                } else {
					//如果下一个delta区间已经在滚动范围end之外了，或者从end往后都是平的，直接在end上插进去一个新delta
                    interval_end = end;
                    insert_delta(end,delta_it->second);
                }
                //更新L真实值
				if (interval_start - 1 >= x_L && interval_end <= x_L)
					dp_L_actual += delta_it->second;
                //负值delta置0，移出
				auto tit = next(it);
				negative_delta_x.erase(it);
				deltas.erase(delta_it);
				it = tit;
                if(it == negative_delta_x.begin())break;
				--it;
				interval_start=*it;
            }
        }
    }
    //将区间[start,end]内的值+1
    void add(int start, int end){
    	//如果x位置已有delta，对其进行添加；如果没有（相当于delta为0），插入新delta
        auto update=[&](int x,int delta){//delta could only be +1 or -1
            auto it=deltas.find(x);
			if(it!=deltas.end())
            	add_delta(it,delta);
			else
				insert_delta(x,delta);
        };
        if(start<end){
            update(start,1);
            update(end+1,-1);
			if(start<=x_L && x_L<=end)
				dp_L_actual+=1;
        } else{
            update(start+1,-1);
            update(end,1);
			if( end<=x_L && x_L<=start)
				dp_L_actual+=1;
        }
    }
    //找出L，R区间内最小值，就是答案
    int find_min(){
        //找出[L,R]内最小值
        auto itb= deltas.lower_bound(x_L + 1);
        auto ite= deltas.lower_bound(x_R);
        int actual,ret;
        actual=ret=dp_L_actual;
        for(auto k=itb;k!=ite;k++){
            actual+=k->second;
            ret=min(actual,ret);
        }
        return ret;
    }
};
int main(){
    int L,R,N;
    cin >> L >> R >> N;
    vector<Tarp> data(N+1);
    vector<allx_stru>allX;
    //将地板作为0号tarp放入data
    data[0]=Tarp(L,0,R,0);
    //加入所有tarp断点的x坐标,不包括L和R
    for(int i=1;i<=N;i++){
        int x1, x2, y1, y2;
        cin>>x1>>y1>>x2>>y2;
        data[i]=Tarp(x1,y1,x2,y2);
        allX.emplace_back(min(x1,x2),i,in);
        allX.emplace_back(max(x1,x2),i,out);
    }
    std::sort(allX.begin(), allX.end(), [] (const allx_stru& a, const allx_stru& b) { return a.x < b.x; });

    //dependency内存储序号n的tarp(不包括0号地板)被哪些序号的tarp依赖,用于拓扑排序
    vector<vector<int>> dependency(N+1);
    vector<int> dep_degree(N+1,0);
	auto cal_y=[&](int x,int x1,int y1,int x2,int y2){
		return double (y1-y2)/(x1-x2)*(x-x1)+y1;
	};
	int nowx;
	auto cmp_Set = [&](const int& t1,const int& t2){
        double h1= cal_y(nowx,data[t1].X1,data[t1].Y1,data[t1].X2,data[t1].Y2);
        double h2= cal_y(nowx,data[t2].X1,data[t2].Y1,data[t2].X2,data[t2].Y2);
		return h1<h2;
	};
    set<int, decltype(cmp_Set)>scan_set(cmp_Set);
    for(auto xstru : allX){
    	nowx=xstru.x;
        if(xstru.direction==in){
            auto insert_pos=scan_set.insert(xstru.index).first;
			if(next(insert_pos)!=scan_set.end()){
                dependency[xstru.index].push_back(*next(insert_pos));
				++dep_degree[*next(insert_pos)];
			}
			if(insert_pos!=scan_set.begin()){
                dependency[*prev(insert_pos)].push_back(xstru.index);
				++dep_degree[xstru.index];
			}
        } else{
        	auto it=scan_set.find(xstru.index);
            if(it!=scan_set.begin() && next(it)!=scan_set.end()) {
				dependency[*prev(it)].push_back(*next(it));
				++dep_degree[*next(it)];
			}
            scan_set.erase(it);
        }
    }

    //输出一个对tarp拓扑排序后的序列
    vector<int> topo_sorted;
    queue<int> deg0que;
    for(int i=1;i<=N;++i)
    	if(dep_degree[i]==0)
    		deg0que.push(i);
	while(!deg0que.empty()){
		int top=deg0que.front();
		topo_sorted.push_back(top);
		deg0que.pop();
		for(int i=0;i<dependency[top].size();++i){
			--dep_degree[dependency[top][i]];
			if(dep_degree[dependency[top][i]]==0)
				deg0que.push(dependency[top][i]);
		}
	}

    //动态规划求解
    dp_solver solver(L,R);
	for(auto it=topo_sorted.rbegin();it!=topo_sorted.rend();it++){
        int x1,x2;
        x1=data[*it].X1;
        x2=data[*it].X2;
        solver.roll(x2,x1);
		solver.add(x2+(x2<x1?1:-1),x1+(x2<x1?-1:1));
	}
    int ans=solver.find_min();
    cout<<ans<<endl;
}