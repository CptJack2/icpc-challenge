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
		//phase 1, query active
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
			//if active has odd components, query with left over, or put it to left over
			if(it!=active.end())
				if(!leftOver.empty()) {//at most 2 elements in leftOver and component in active always has more elements
					queries[it->front()] = leftOver.front().front();
					queries[leftOver.front().front()] =it->front();
				}else{
					leftOver.splice(leftOver.end(),active,it);
					it=active.end();
				}
			auto res = query(queries);//do query
			//merge the remaining active with left over
			if(it!=active.end() && !leftOver.empty()){
				if(res[it->front()] == res[leftOver.front().front()] && res[it->front()]==true)
					leftOver.front().splice(leftOver.front().end(),*it),
					active.erase(it);
				else if(leftOver.size()==2)
					next(leftOver.begin())->splice(leftOver.front().end(),*it),
					active.erase(it);
				else
					leftOver.splice(leftOver.end(),active,it);
			}
			//put active pair to paired or group them together according to query result
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
		while(!paired.empty()){
			vector<int> queries(n+1,0);
			auto git = good.begin();
			auto pit = paired.begin();
			if (good.size() - bad.size() <= 2) {
				for (; git != good.end() && pit!=paired.end();++git,++pit)
					queries[*git]=pit->first.front();
				auto res=query(queries);
				git = good.begin();
				pit = paired.begin();
				for (; git != good.end() && pit!=paired.end();++git) {
					if (res[*git])
						good.splice(good.end(), pit->first),
						bad.splice(bad.end(), pit->second);
					else
						good.splice(good.end(), pit->second),
						bad.splice(bad.end(), pit->first);
					auto tpit=next(pit);
					paired.erase(pit);
					pit=tpit;
				}
			}else{
				for(int i=0;i<good.size()/2;++i,++pit,advance(git,2))
					queries[*git]=pit->first.front(),
					queries[*next(git)]=pit->second.front();
				auto res=query(queries);
				pit=paired.begin();
				git=good.begin();
				for(int i=0;i<good.size()/2;++i,++pit,advance(git,2)){
					if(res[*git])
						good.splice(good.end(),pit->first);
					else
						bad.splice(bad.end(),pit->first);
					if(res[*next(git)])
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