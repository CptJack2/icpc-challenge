#include "bits/stdc++.h"
using namespace std;

typedef list<int> Component;//machines with same state

int main(){
	int batch;
	cin>>batch;
	for (int i = 0; i < batch; ++i) {
		int n;
		cin>>n;
		list<Component> active,leftOver;
		list<pair<Component,Component>> paired;
		for (int j = 1; j <= n; ++j) {
			Component tc;
			tc.push_back(j);
			active.push_back(tc);
		}
		auto query = [&](const vector<int> &queries) -> vector<bool> {
			cout << "test";
			for (auto &q:queries)
				cout << " " << q;
			vector<bool> ret(n+1, false);
			char c;
			for (int i = 1; i <= n; ++i)
				cin >> c,
				ret[queries[i]] = c == '1';
			return ret;
		};
		//phase 1, query active
		while(active.size()>1) {
			auto it = active.begin(), nit = it;
			vector<int> queries(n+1,0);
			while (1) {
				nit = next(it);
				if (nit == active.end()) {
					leftOver.splice(leftOver.end(), active, it);
					break;
				}
				queries[it->front()] = nit->front();
				it = next(nit);
				if (it == active.end())break;
			}
			if(it!=active.end())
				if(!leftOver.empty()) {//at most 2 elements in leftOver and component in active always has more elements
					queries[it->front()] = leftOver.front().front();
					queries[leftOver.front().front()] =it->front();
//					if (leftOver.size() == 2) //deal with the second left over component
//						queries[*next(it->begin())] = next(leftOver.begin())->front(),
//						queries[next(leftOver.begin())->front()] = *next(it->begin());
				}else{
					leftOver.splice(leftOver.end(),active,it);
					active.erase(it);
					it=active.end();
				}
			auto res = query(queries);
			if(it!=active.end() && !leftOver.empty()){
				if(res[it->front()] == res[leftOver.front().front()] && res[it->front()]==true)
					leftOver.front().splice(leftOver.front().end(),*it);
				else if(leftOver.size()==2)
					next(leftOver.begin())->splice(leftOver.front().end(),*it);
				else
					leftOver.splice(leftOver.end(),active,it);
				active.erase(it),
				it=active.end();
			}
			it = active.begin();
			while (it!=active.end()) {
				nit = next(it);
				if(res[it->front()]==res[nit->front()] && res[it->front()]==true){
					it->splice(it->begin(),*nit);
					active.erase(nit);
					it=next(it);
				}else{
					paired.emplace_back(*it,*nit);
					auto tit=next(nit);
					active.erase(it);
					active.erase(nit);
					it=tit;
				}
			}
		}
		//intermediate phase
		Component good,bad;
		vector<int> queries(n,0);
		if(active.size()==1){
			good.splice(good.begin(),active.front());
			active.clear();
			if(!leftOver.empty()){
				queries[good.front()]=leftOver.front().front();
				if(leftOver.size()==2)
					queries[*next(good.begin())]=next(leftOver.begin())->front();
				auto res=query(queries);
				if(res[leftOver.front().front()])
					good.splice(good.end(),leftOver.front());
				else
					bad.splice(bad.end(),leftOver.front());
				if(leftOver.size()==2)
					if(res[next(leftOver.begin())->front()])
						good.splice(good.end(),*next(leftOver.begin()));
					else
						bad.splice(bad.end(),*next(leftOver.begin()));
				leftOver.clear();
			}
		}else{
			auto biggestLeftOver=leftOver.begin();
			for(auto it=leftOver.begin();it!=leftOver.end();++it)
				if(it->size() > biggestLeftOver->size())
					biggestLeftOver=it;
			good.splice(good.end(),*biggestLeftOver);
			leftOver.erase(biggestLeftOver);
			for(auto& l:leftOver)
				bad.splice(bad.end(),l);
			leftOver.clear();
		}
		//phase 2

	}
	return 0;
}