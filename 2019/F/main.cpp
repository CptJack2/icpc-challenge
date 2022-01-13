#include <vector>
#include <list>
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
	//X cord->delta storage pointer
    map<int, list<pair<int,int>>::iterator> delta_index;
    list<pair<int,int>> delta_storage;
    int dp_L_actual,x_L,x_R;
    void update_delta(int x, int delta){
        auto index_it=delta_index.lower_bound(x);
        if(index_it==delta_index.end()){
            delta_storage.emplace_back(make_pair(x,delta));
            auto tailIt=prev(delta_storage.end());
            delta_index[x]=tailIt;
            return;
        }
        auto sotrage_it=index_it->second;
        if(sotrage_it->first == x)
            sotrage_it->second=delta;
        else{
            auto newIt=delta_storage.insert(sotrage_it, make_pair(x, delta));
            delta_index[x]=newIt;
        }
    }
    list<pair<int,int>>::iterator delete_delta(list<pair<int,int>>::iterator where){//return next iter of deleted
        auto tit=next(where);
        int x=where->first;
        delta_storage.erase(where);
        delta_index.erase(x);
        return tit;
    }
    dp_solver(int L,int R): x_L(L), x_R(R),dp_L_actual(0){
        update_delta(L, -inf);
        update_delta(R, inf);
    }
    void roll(int start, int end){
        int interval_start=start;
    	if(interval_start < end){
        	if(debug_dp)
				for(int k=interval_start; k <= end - 1; ++k)
					if(dp[k-minx+1]>=dp[k-minx])
						dp[k-minx+1]=dp[k-minx];
            auto index_iter= delta_index.lower_bound(interval_start + 1);
            if(index_iter==delta_index.end())return;
            auto storage_iter=index_iter->second;
            interval_start=storage_iter->first;
            while(interval_start <= end){
                if(storage_iter->second>0) {
                	int interval_end;
                    if (next(storage_iter) !=delta_storage.end() && next(storage_iter)->first <= end + 1) {
						next(storage_iter)->second += storage_iter->second;
						interval_end= next(storage_iter)->second-1;
					}else{
                        update_delta(end + 1, storage_iter->second);
						interval_end= end;
					}
                    if(interval_start<=x_L && interval_end>=x_L)
                    	dp_L_actual-=storage_iter->second;
                    storage_iter= delete_delta(storage_iter);
                } else{
                    ++storage_iter;
                    if(storage_iter== delta_storage.end())break;
                }
				interval_start=storage_iter->first;
            }
        } else{
        	if(debug_dp)
				for(int k=interval_start; k >= end + 1; --k)
					if(dp[k-minx-1]>=dp[k-minx])
						dp[k-minx-1]=dp[k-minx];
            list<pair<int,int>>::iterator storage_iter;
            auto index_iter=delta_index.lower_bound(interval_start + 1);
            if(index_iter==delta_index.end())
                storage_iter=prev(delta_storage.end());
            else
                storage_iter=prev(index_iter->second);
            if(storage_iter==delta_storage.begin())return;
			interval_start=storage_iter->first;
            while(interval_start > end){
				int interval_end;
                if(storage_iter->second<0) {
                    if (storage_iter!=delta_storage.begin() && prev(storage_iter)->first >= end) {
                        prev(storage_iter)->second += storage_iter->second;
                        interval_end=prev(storage_iter)->first;
                        storage_iter= delete_delta(storage_iter);
                    } else {
                        update_delta(end, storage_iter->second);
						interval_end=end;
                        storage_iter= delete_delta(storage_iter);
                    }
                    if(interval_start-1>=x_L && interval_end<=x_L)
						dp_L_actual+=storage_iter->second;
                }
				if(storage_iter==delta_storage.begin())break;
				--storage_iter;
				interval_start=storage_iter->first;
            }
        }
    }
    void add(int start, int end){
        auto update=[&](int x,int delta){
            auto it= delta_index.find(x);
            if(it!=delta_index.end())
                it->second->second+=delta;
            else
                update_delta(x,delta);
        };
        if(start<end){
            update(start,1);
            update(end+1,-1);
			if(start<=x_L && x_L<=end)
				dp_L_actual+=1;
			if(debug_dp)
				for(int k=start;k<=end;++k)
					dp[k-minx]+=1;
        } else{
            update(start+1,-1);
            update(end,1);
			if( end<=x_L && x_L<=start)
				dp_L_actual+=1;
			if(debug_dp)
				for(int k=start;k>=end;--k)
					dp[k-minx]+=1;
        }
    }
    int find_min(){
        auto itb= delta_index.lower_bound(x_L+1)->second;
        auto ite= delta_index.lower_bound(x_R)->second;
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
			dp.resize(maxx+1-minx,inf);
			for(int i=L;i<=R;++i)
				dp[i-minx]=0;
		}
		else {
			debug_dp= false;
		}
	}
    dp_solver solver(L,R);
	for(int i=topo_sorted.size()-1;i>=0;--i){
        int x1,x2;
        x1=data[topo_sorted[i]].X1;
        x2=data[topo_sorted[i]].X2;
        solver.roll(x2,x1);
		solver.add(x2+(x2<x1?1:-1),x1+(x2<x1?-1:1));
	}
    int ans=solver.find_min();
    cout<<ans<<endl;
}