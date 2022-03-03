#include "bits/stdc++.h"
using namespace std;

int n,m;
vector<set<int>> graph;
vector<bool> visited;

vector<int> find_connected_componet(int src){
	vector<int> ret;
	stack <int> s;

	if(!visited[src]){
		visited[src] = true;
		s.push(src);
	}
	else
		return ret;

	while(!s.empty()){
		int s_top =  s.top();
		ret.push_back(s_top);
		s.pop();
		for(auto v:graph[s_top])
			if(!visited[v]){
				visited[v]=true;
				s.push(v);
			}
	}
	return ret;
}
int total_degrees(const vector<int>& nodes){
	int ret=0;
	for(auto v: nodes)
		ret+=graph[v].size();
	return ret;
}
int main(){
	cin>>n>>m;
	graph.resize(n+1);
	visited.resize(n+1, false);
	for (int i = 0; i < m; ++i) {
		int v,w;
		cin>>v>>w;
		graph[v].insert(w);
		graph[w].insert(v);
	}
	vector<pair<int,int>>ret;
	for (int i = 1; i <= n; ++i) {
		if(visited[i])continue;
		auto connectedComponet=find_connected_componet(i);
		int totalDegree= total_degrees(connectedComponet);
		//连通分量是树
		if(totalDegree==2*(connectedComponet.size()-1)){
			//所有叶子节点的出路放置标志
			for(auto v:connectedComponet)
				if(graph[v].size()==1)
					ret.emplace_back(v,*graph[v].begin());
		} else{
			map<int,int> nonLeaf;//node index -> degree
			map<int,set<int>> degreeIndex;//degree -> index set
			auto maintainDegreeIndex=[&](int index,int oldDegree, int newDegree){
				if(oldDegree!=0){
					degreeIndex[oldDegree].erase(index);
				}
				degreeIndex[newDegree].insert(index);
			};
			for(int v : connectedComponet){
				nonLeaf[v]=graph[v].size();
				maintainDegreeIndex(v,0,graph[v].size());
			}
			while(degreeIndex.count(1) && !degreeIndex[1].empty()){
				auto t=degreeIndex[1];
				for(auto v:t) {
					//leaf node, remove and update degree
					for (auto v2:graph[v])
						if (nonLeaf.count(v2)) {
							maintainDegreeIndex(v2, nonLeaf[v2], nonLeaf[v2] - 1);
							--nonLeaf[v2];
						}
					nonLeaf.erase(v);
					degreeIndex[1].erase(v);
				}
			}
			for(auto v:nonLeaf)
				for (auto v2:graph[v.first])
					if(!nonLeaf.count(v2))
						ret.emplace_back(v.first,v2);
		}
	}
	sort(ret.begin(),ret.end());
	cout<<ret.size()<<endl;
	for(auto r:ret)
		cout<<r.first<<" "<<r.second<<endl;
}