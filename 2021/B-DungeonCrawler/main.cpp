#include<bits/stdc++.h>
using namespace std;

int n,q;
vector<vector<pair<int,int>>> graph;//邻接点 边长度
vector<vector<pair<int,unsigned long long>>> longestPaths;//离开节点，最长路径长度。节点数上限2000，边长度上限10^9，最长路径上限2x10^12，需要用超过int的整型
vector<int> depth;//根高度为0
unsigned getLongest(int node, int exclusion1, int exclusion2){
    for(auto [u,w]:longestPaths[node])
        if(u!=exclusion1 && u!=exclusion2)
            return w;
}
unsigned long long dfs1ComputeChildLongestAndDepth(int node, int parent, int depthNode){//返回从node能到达的最长路径长度
    depth[node]=depthNode;
    unsigned long long ret=0;
    for(auto [u,w]:graph[node])
        if(u!=parent) {
            unsigned long long longestPathCh=w + dfs1ComputeChildLongestAndDepth(u, node, depthNode + 1);
            longestPaths[node].push_back({u,longestPathCh});
            ret = max(ret, longestPathCh);
        }else{
            longestPaths[node].push_back({parent,-1});
        }
    return ret;
}
void dfs2(int node, int parent, int parentLongest){
    for(auto& [_,w]:longestPaths[node])
        if(w==-1)
            w=parentLongest;
    sort(longestPaths[node].begin(),longestPaths[node].end(),[](auto& a, auto& b){return a.second>b.second;});
    for(auto [u,w]:graph[node])
        if(u!=parent)
            dfs2(u,node,w+ getLongest(node, u, -1));
}
void computeLongestPathForEachNode(){
    dfs1ComputeChildLongestAndDepth(0,-1,0);
    dfs2(0, -1, 0);
}

int main(){
    cin>>n>>q;
    graph.resize(n+1);
    longestPaths.resize(n+1);
    depth.resize(n+1);
    for(int i=0;i<n;++i){
        int u,v,w;
        cin>>u>>v>>w;
        graph[u].push_back({v,w});
        graph[v].push_back({u,w});
    }
    computeLongestPathForEachNode();
    for(int i=0;i<q;++i){
        int s,k,t;
        cin>>s>>k>>t;
    }
}