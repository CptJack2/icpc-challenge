#include "bits/stdc++.h"
using namespace std;

int n,m;
vector<set<int>> graph;
void find_connected_componet(int index,int parent, vector<int>& ret, vector<bool>& visited, bool& is_tree){
	if(visited[index])return;
	visited[index]=true;
//	if(is_tree)
//		for(auto v:ret){
//			//新加入的节点有除了来的路之外,能到达已加入的节点的其他路径,
//			if(graph[index].count(v) && parent!=v)
//				is_tree=false;
//		}
	ret.push_back(index);
	for(auto i: graph[index]){
		if(!visited[i])
			find_connected_componet(i,index,ret,visited,is_tree);
		else if(is_tree)
			//新加入的点能到达已访问的点,此图不是树
			is_tree= false;
	}
}
int main(){
	cin>>n>>m;
	graph.resize(n+1);
	for (int i = 0; i < m; ++i) {
		int v,w;
		cin>>v>>w;
		graph[v].insert(w);
		graph[w].insert(v);
	}
	vector<pair<int,int>>ret;
	for (int i = 1; i <= n; ++i) {

	}
}