#include <vector>
#include "iostream"
#include "queue"
#include "algorithm"
#include "set"
#include "map"
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
vector<int> dp;
bool debug_dp=true;
int minx,maxx;
class dp_solver{
public:
	//X cord->delta
    map<int, int> pos_deltas;
    map<int, int> neg_deltas;
    int dp_L_actual,x_L,x_R;
    dp_solver(int L,int R): x_L(L), x_R(R),dp_L_actual(0){
    	neg_deltas[L]=-inf;
    	pos_deltas[R + 1]= inf;
    }
    void add_delta(map<int,int>::iterator where,int num){
        int old_delta=where->second;
        auto& old_delta_map= old_delta>0?pos_deltas:neg_deltas;
        auto& new_delta_map= old_delta<0?pos_deltas:neg_deltas;
        where->second+=num;
        //判断delta修改后是否异号. int二进制,负数最高位是1,正数0。负数^正数最高位为1
        if(where->second==0){
            old_delta_map.erase(where);
        } else if((old_delta ^ where->second) < 0){
            new_delta_map[where->first]=where->second;
            old_delta_map.erase(where);
        }
    }
    void roll(int start, int end){
        int interval_start=start;
    	if(interval_start < end){
        	if(debug_dp)
				for(int k=interval_start; k <= end - 1; ++k)
					if(dp[k+1]>=dp[k])
						dp[k+1]=dp[k];
            auto it= pos_deltas.lower_bound(interval_start + 1);
            if(it==pos_deltas.end())return;
			interval_start=it->first;
            while(interval_start <= end){
                int interval_end;
                if (next(it) != pos_deltas.end() && next(it)->first <= end + 1) {
                    add_delta(next(it),it->second);
                    interval_end = next(it)->second - 1;
                } else {
                    pos_deltas[end + 1] = it->second;
                    interval_end = end;
                }
                if (interval_start <= x_L && interval_end >= x_L)
                    dp_L_actual -= it->second;
                auto tit = next(it);
                pos_deltas.erase(it);
                it = tit;
                interval_start=it->first;
            }
        } else{
        	if(debug_dp)
				for(int k=interval_start; k >= end + 1; --k)
					if(dp[k-1]>=dp[k])
						dp[k-1]=dp[k];
            auto it=neg_deltas.lower_bound(interval_start + 1);
            if(it==neg_deltas.begin())return;
            --it;
			interval_start=it->first;
            while(interval_start > end){
				int interval_end;
                if (it != neg_deltas.begin() && prev(it)->first >= end) {
                    //更新前一区间delta
                    add_delta(prev(it),it->second);
                    interval_end = prev(it)->first;
                    auto tit = next(it);
                    neg_deltas.erase(it);
                    it = tit;
                } else {
                    neg_deltas[end] = it->second;
                    interval_end = end;
                    auto tit = next(it);
                    neg_deltas.erase(it);
                    it = tit;
                }
                if (interval_start - 1 >= x_L && interval_end <= x_L)
                    dp_L_actual += it->second;
                if(it==neg_deltas.begin())break;
				--it;
				interval_start=it->first;
            }
        }
    }
    void add(int start, int end){
        auto update=[&](int x,int delta){//delta could only be +1 or -1
            auto it= pos_deltas.find(x);
            if(it==pos_deltas.end()){
                it=neg_deltas.find(x);
                if(it==neg_deltas.end()){
                    if(delta>0)
                        pos_deltas[x]=delta;
                    else
                        neg_deltas[x]=delta;
                    return ;
                }
            }
            add_delta(it,delta);
        };
        if(start<end){
            update(start,1);
            update(end+1,-1);
			if(start<=x_L && x_L<=end)
				dp_L_actual+=1;
			if(debug_dp)
				for(int k=start+1;k<=end;++k)
					dp[k]+=1;
        } else{
            update(start+1,-1);
            update(end,1);
			if( end<=x_L && x_L<=start)
				dp_L_actual+=1;
			if(debug_dp)
				for(int k=start-1;k>=end;--k)
					dp[k]+=1;
        }
    }
    int find_min(){
        //先把neg_delta对应区间的copy到pos delta里
        for(auto it=neg_deltas.lower_bound(x_L); it!=neg_deltas.upper_bound(x_R); ++it)
            pos_deltas[it->first]=it->second;
        auto itb= pos_deltas.lower_bound(x_L+1);
        auto ite= pos_deltas.lower_bound(x_R);
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
    data[0]=Tarp(L,0,R,0);
    for(int i=1;i<=N;i++){
        int x1, x2, y1, y2;
        cin>>x1>>y1>>x2>>y2;
        data[i]=Tarp(x1,y1,x2,y2);
        allX.emplace_back(min(x1,x2),i,in);
        allX.emplace_back(max(x1,x2),i,out);
    }
    std::sort(allX.begin(), allX.end(), [] (const allx_stru& a, const allx_stru& b) { return a.x < b.x; });

    vector<vector<int>> dependency(N+1);
    vector<int> dep_degree(N+1,0);
	auto cal_y=[&](int x,int x1,int y1,int x2,int y2){
		return float(y1-y2)/(x1-x2)*(x-x1)+y1;
	};
	int nowx;
	auto cmp_Set = [&](const int& t1,const int& t2){
		float h1= cal_y(nowx,data[t1].X1,data[t1].Y1,data[t1].X2,data[t1].Y2);
		float h2= cal_y(nowx,data[t2].X1,data[t2].Y1,data[t2].X2,data[t2].Y2);
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
	if(debug_dp){
		if(!allX.empty()) {
			minx=allX[0].x,maxx=allX.back().x;
			dp.resize(maxx+1,inf);
			for(int i=L;i<=R;++i)
				dp[i]=0;
		}
		else {
			debug_dp= false;
		}
	}
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