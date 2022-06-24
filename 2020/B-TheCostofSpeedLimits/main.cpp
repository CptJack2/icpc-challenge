#include "bits/stdc++.h"

using namespace std;

vector<map<int,int>> graph;
vector<int> allSpeed;
int n,c;

pair<int,vector<int>> dfs(int node, int parent){
	int signCost=graph[node].size()>1?c*graph[node].size():0;
	vector<int> costs(allSpeed.size(),0);
	int maxSpeed=0, parS=graph[node][parent];
	for(auto [ch,s]:graph[node])maxSpeed=max(maxSpeed,s);
	for(auto [ch,s]:graph[node])
		if(ch!=parent){
			auto [chBC,chC]=dfs(ch,node);
			signCost+=chBC;
			for(int i=0;i<allSpeed.size();i++)
				costs[i]+=chC[i]+allSpeed[i]-s;
		}
	int bestCost=signCost;
	for(int i=0;i<allSpeed.size();i++)
		if(allSpeed[i]<maxSpeed)
			costs[i]=signCost+allSpeed[i]-parS;
		else
			costs[i]+=allSpeed[i]-parS,
			bestCost=min(bestCost,costs[i]),
			costs[i]=min(costs[i],signCost+allSpeed[i]-parS);
	return {bestCost,costs};
}

int main(){
	cin>>n>>c;
	graph.resize(n + 1);
	set<int> uniqueSpeeds;
	for(int i=0;i<n-1;i++){
		int u,v,s;
		cin>>u>>v>>s;
		graph[u][v]=s;
		graph[v][u]=s;
		uniqueSpeeds.insert(s);
	}
	for(auto s:uniqueSpeeds)allSpeed.push_back(s);
	graph[1][0]=0;
	auto [bc,_]=dfs(1,0);
	cout<<bc;
	return 0;
}