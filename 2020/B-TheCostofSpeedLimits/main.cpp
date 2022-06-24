#include "bits/stdc++.h"

using namespace std;

vector<map<int,int>> speeds;

pair<int,vector<int>> dfs(int node, int parent){
	for(auto [ch,s]:speeds[node])
		if(ch!=parent){
			auto chR=dfs(ch,node);

		}
}

int main(){
	int n,c;
	cin>>n>>c;
	speeds.resize(n+1);
	for(int i=0;i<n-1;i++){
		int u,v,s;
		cin>>u>>v>>s;
		speeds[u][v]=s;
		speeds[v][u]=s;
	}
	dfs(1,0);
	return 0;
}