#include "bits/stdc++.h"
using namespace std;

typedef pair<int,int> pii;
int longest=0;
vector<pii> longestPath;
vector<vector<pii>> g;

pair<int,vector<pii>> findLongestPath(int cur,int parent,bool findPath){
	int h1=0,h2=0;
	vector<pii> vh1,vh2;
	for(auto& [ch,d]:g[cur]){
		if(ch==parent)continue;
		auto [hch,vhch]= findLongestPath(ch,cur,findPath);
		int h=hch+d;
		if(h>h1)
			h2=h1,
			h1=h,
			vh2=vh1,
			vhch.emplace_back(cur,ch),
			vh1=vhch;
		else if(h>h2)
			vhch.emplace_back(cur,ch),
			vh2=vhch,
			h2=h;
	}
	if(h1+h2>longest){
		longest=h1+h2;
		if(findPath) {
			vector<pii> newlp(h1 + h2);
			copy(vh1.begin(), vh1.end(), newlp.begin());
			copy(vh2.rbegin(), vh2.rend(), newlp.begin() + h1);
		}
	}
	return {h1,vh1};
}

int main(){
	int n;
	cin>>n;
	g.resize(n+1);
	int total=0;
	for (int i = 0; i < n; ++i) {
		int a,b,d;
		cin>>a>>b>>d;
		g[a].emplace_back(b,d);
		total+=2*d;
	}
	longest=0,longestPath.clear();
	findLongestPath(1,0, true);
	total-=longest;
	for(auto edge:longestPath){
		for(auto it=g[edge.first].begin();it!=g[edge.first].end();++it)
			if(it->first==edge.second) {
				g[edge.first].erase(it);
				break;
			}
		for(auto it=g[edge.second].begin();it!=g[edge.second].end();++it)
			if(it->first==edge.first) {
				g[edge.second].erase(it);
				break;
			}
	}
	longest=0,longestPath.clear();
	findLongestPath(1,0, false);
	cout<<total-longest;
	return 0;
}