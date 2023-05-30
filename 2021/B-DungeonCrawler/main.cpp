#include<bits/stdc++.h>
using namespace std;

int n,q;
vector<vector<pair<int,int>>> graph;//邻接点 边长度
vector<vector<pair<int,unsigned long long>>> longestPaths;//离开节点，最长路径长度。节点数上限2000，边长度上限10^9，最长路径上限2x10^12，需要用超过int的整型
vector<int> depth;//根高度为0
unsigned long long getLongest(int node, int exclusion1, int exclusion2){
    for(auto [u,w]:longestPaths[node])
        if(u!=exclusion1 && u!=exclusion2)
            return w;
    return 0;
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
void dfs2ComputeParentLongestPath(int node, int parent, int parentLongest){
    for(auto& [_,w]:longestPaths[node])
        if(w==-1)
            w=parentLongest;
    sort(longestPaths[node].begin(),longestPaths[node].end(),[](auto& a, auto& b){return a.second>b.second;});
    for(auto [u,w]:graph[node])
        if(u!=parent)
            dfs2ComputeParentLongestPath(u,node,w+ getLongest(node, u, -1));
}
void computeLongestPathForEachNode(){
    dfs1ComputeChildLongestAndDepth(0,-1,0);
    dfs2ComputeParentLongestPath(0, -1, 0);
}
vector<vector<int>> ancestors, ancestorSuccessor;
vector<vector<unsigned long long>> ancDist;
vector<vector<unsigned long long>> upNotOverlap, upOverlap, downNotOverlap, downOverlap;
void dfs3(int node, int parent, int w){
    auto dep=depth[node];
    int lv=1;
    while((dep<<=1&1)==0)
        ++lv;
    ancestors[node].resize(lv); ancestorSuccessor[node].resize(lv);
    upNotOverlap[node].resize(lv); upOverlap[node].resize(lv);
    downNotOverlap[node].resize(lv); downOverlap[node].resize(lv);

    ancestors[node][0]=parent; ancestorSuccessor[node][0]=node; ancDist[node][0]=node;
    upNotOverlap[node][0]=w; downNotOverlap[node][0]=w;
    upOverlap[node][0]=0; downOverlap[node][0]=0;
    for(int i=1;i<lv;++i){
        auto anc=ancestors[node][i-1];
        auto ancLongest=getLongest(anc, ancestorSuccessor[node][i-1], ancestors[anc][0]);

        ancestors[node][i]=ancestors[anc][i-1];
        ancestorSuccessor[node][i]=ancestorSuccessor[anc][i-1];
        ancDist[node][i]=ancDist[node][i-1]+ancDist[anc][i-1];

        upNotOverlap[node][i]=max(upNotOverlap[node][i-1], ancDist[node][i-1] + max(upNotOverlap[anc][i-1], ancLongest));
        downNotOverlap[node][i]=max(downNotOverlap[anc][i-1], ancDist[anc][i-1] + max(downNotOverlap[node][i-1], ancLongest));
        upOverlap[node][i]=max(upOverlap[node][i-1], -ancDist[node][i-1] + max(upOverlap[anc][i-1], ancLongest));
        downOverlap[node][i]=max(downOverlap[anc][i-1], -ancDist[anc][i-1] + max(downOverlap[node][i-1], ancLongest));
    }
    for(auto [u,w]:graph[node])
        if(u!=parent)
            dfs3(u,node,w);
}
void dynamicProgrammingPathData(){
    ancestors.resize(n+1); ancestorSuccessor.resize(n+1); ancDist.resize(n+1);
    upNotOverlap.resize(n+1); upOverlap.resize(n+1);
    downNotOverlap.resize(n+1); downOverlap.resize(n+1);
    for(auto [u,w]:graph[0])
        dfs3(u,0,w);
}
vector<pair<int,int>> findCommonAncestors(int n1, int n2){
    auto d1=depth[n1], d2=depth[n2];
    vector<pair<int,int>> ret;
    while(d2>d1){
        for(auto i=ancestors[n2].size()-1;i>=0;--i)
            if(d2-(1<<i)>=d1) {
                d2 -= 1 << i;
                n2 = ancestors[n2][i];
                break;
            }
    }
    while(d1>d2){
        for(auto i=ancestors[n1].size()-1;i>=0;--i)
            if(d1-(1<<i)>=d2) {
                d1 -= 1 << i;
                ret.push_back({n1,i});
                n1 = ancestors[n1][i];
                break;
            }
    }
    while(d1!=d2){
        for(auto i=ancestors[n1].size()-1;i>=0;--i)
            if(d1-(1<<i)>=d2) {
                d1 -= 1 << i;
                d2-=1<<i;
                ret.push_back({n1,i});
                n1 = ancestors[n1][i];
                n2 = ancestors[n2][i];
                break;
            }
    }
    return ret;
}
unsigned long long totalLength=0;
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
        totalLength+=w;
    }
    computeLongestPathForEachNode();
    for(int i=0;i<q;++i){
        int s,k,t;
        cin>>s>>k>>t;
        auto psk=findCommonAncestors(s,k), pst= findCommonAncestors(s,t), pks= findCommonAncestors(k,s), pkt= findCommonAncestors(k,t);
        auto path1=depth[psk.back().first] > depth[pst.back().first]?psk:pst;//从S到SK和ST的交点J或者SK最高的共同祖先
        auto path2= findCommonAncestors(path1.back().first, k);//从J到SK最高的共同祖先，可能为空
        auto path4= depth[pks.back().first] > depth[pkt.back().first]?pks:pkt;//从SK最高的共同祖先到J，可能为空，path2和path3必定有一个为空，另一个非空
        auto path3= findCommonAncestors(path4.back().first,s);//从J到K点
        //如果T就是J点，那么必然要先经过T才能到S，输出impossible
        if(path1.back().first==t || path4.back().first==t){
            cout<<"impossible"<<endl;
            continue;
        }
        unsigned long long base=0, ret=0;
        int succ=-1,node=s;
        for(int j=0;j<path1.size()-2;++j){
            node=path1[j].first;
            auto exp=path1[j].second;
            ret=max(ret, base+ getLongest(node,ancestors[node][0],succ));
            ret=max(ret, base+ upNotOverlap[node][exp]);
            base+=ancDist[node][exp];
            succ=ancestorSuccessor[node][exp];
        }
        for(int j=0;j<path2.size()-2;++j){
            node=path2[j].first;
            auto exp=path2[j].second;
            ret=max(ret, base+ getLongest(node,ancestors[node][0],succ));
            ret=max(ret, base+ upOverlap[node][exp]);
            base-=ancDist[node][exp];
            succ=ancestorSuccessor[node][exp];
        }
        node=path2.back().first;
        for(int j=path3.size()-2;j>=0;--j){
            auto nextNode=path3[j].first, exp=path3[j].second;
            ret=max(ret, base+ getLongest(node,ancestorSuccessor[nextNode][exp],succ));
            ret=max(ret, base+ downOverlap[node][exp]);
            base+=ancDist[nextNode][exp];
            succ=ancestorSuccessor[nextNode][0];
            node=nextNode;
        }
        for(int j=path4.size()-2;j>=0;--j){
            auto nextNode=path4[j].first, exp=path4[j].second;
            ret=max(ret, base+ getLongest(node,ancestorSuccessor[nextNode][exp],succ));
            ret=max(ret, base+ downNotOverlap[node][exp]);
            base-=ancDist[nextNode][exp];
            succ=ancestorSuccessor[nextNode][0];
            node=nextNode;
        }
        ret=max(ret, base+ getLongest(node, succ, -1));
        cout<<2*totalLength-ret<<endl;
    }
}