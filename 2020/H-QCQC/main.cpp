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
		//each machine form a component at the beginning
		for (int j = 1; j <= n; ++j) {
			Component tc;
			tc.push_back(j);
			active.push_back(tc);
		}
		//interactively query machine state and get result
		auto query = [&](const vector<int> &queries) -> vector<bool> {
			cout << "test";
			for (int i = 1; i <= n; ++i)
				cout << " " << queries[i];
			cout<<endl;
			vector<bool> ret(n+1, false);
			char c;
			for (int i = 1; i <= n; ++i)
				cin >> c,
				ret[queries[i]] = c == '1';
			return ret;
		};
		//phase 1, group two active component and make them query each other.
		while(active.size()>1) {
			auto it = active.begin(), nit = it;
			vector<int> queries(n+1,0);
			//group active components into pairs for query
			while (1) {
				nit = next(it);
				if (nit == active.end())
					break;
				queries[it->front()] = nit->front();
				queries[nit->front()] = it->front();
				it = next(nit);
				if (it == active.end())break;
			}
			//if active has odd components, query each other with each left over
			if(it!=active.end()) {
				auto ait = it->begin();
				for (auto lo:leftOver)
					queries[*ait] = lo.front(),
					queries[lo.front()] = *ait,
					++ait;
			}
			auto res = query(queries);
			//merge the remaining active with left over
			if(it!=active.end()){
				for(auto lit=leftOver.begin();lit!=leftOver.end();) {
					auto lo=*lit;
					if (res[lo.front()] == res[queries[lo.front()]] && res[lo.front()] == true) {
						it->splice(it->end(), lo);
						auto tlit=next(lit);
						leftOver.erase(lit);
						lit=tlit;
					}else
						++lit;
				}
				leftOver.splice(leftOver.end(),active,it);
			}
			//put active pair to paired, or group them together according to query result
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
		vector<int> queries(n+1,0);
		//if 1 active is left, it has to be good, and use it to get state of left overs
		if(active.size()==1){
			good.splice(good.begin(),active.front());
			active.clear();
			if(!leftOver.empty()){
				auto git=good.begin();
				for(auto& lo:leftOver)
					queries[*(git++)]=lo.front();
				auto res=query(queries);
				for(auto& lo:leftOver)
					if(res[lo.front()])
						good.splice(good.end(),lo);
					else
						bad.splice(bad.end(),lo);
				leftOver.clear();
			}
		//if no active left, the biggest left over( last left over is the biggest) must be good, and others are bad
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
		//phase 2, use good machines to get result of paired
		while(!paired.empty()){
			vector<int> queries(n+1,0);
			auto git = good.begin();
			auto pit = paired.begin();
			//if number of good is no more than number of bad +2, then each pair contains one good and one bad
			if (good.size() - bad.size() <= 2) {
				for (; git != good.end() && pit!=paired.end();++git,++pit)
					queries[*git]=pit->first.front();
				auto res=query(queries);
				git = good.begin();
				int gsize=good.size();
				pit = paired.begin();
				for (int i=0; i<gsize && pit!=paired.end();++git,++i) {
					if (res[pit->first.front()])
						good.splice(good.end(), pit->first),
						bad.splice(bad.end(), pit->second);
					else
						good.splice(good.end(), pit->second),
						bad.splice(bad.end(), pit->first);
					auto tpit=next(pit);
					paired.erase(pit);
					pit=tpit;
				}
			//otherwise, we have to query every single component to get result
			}else{
				int gsize=good.size();
				for(int i=0;i<gsize/2 && pit!=paired.end();++i,++pit,advance(git,2))
					queries[*git]=pit->first.front(),
					queries[*next(git)]=pit->second.front();
				auto res=query(queries);
				pit=paired.begin();
				git=good.begin();
				for(int i=0;i<gsize/2 && pit!=paired.end();++i,advance(git,2)){
					if(res[pit->first.front()])
						good.splice(good.end(),pit->first);
					else
						bad.splice(bad.end(),pit->first);
					if(res[pit->second.front()])
						good.splice(good.end(),pit->second);
					else
						bad.splice(bad.end(),pit->second);
					auto tpit=next(pit);
					paired.erase(pit);
					pit=tpit;
				}
			}
		}
		//output
		vector<bool> ans(n+1);
		for(auto g:good)
			ans[g]=true;
		for(auto b:bad)
			ans[b]=false;
		cout<<"answer ";
		for (int j = 1; j <=n ; ++j)
			cout<<(ans[j]?1:0);
		cout<<endl;
	}
	return 0;
}