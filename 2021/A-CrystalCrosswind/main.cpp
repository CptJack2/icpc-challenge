#include "bits/stdc++.h"

using namespace std;

int dx, dy, k;
vector<vector<char>> graph;
vector<pair<int,int>> winds;

inline bool inRange(int x, int y){
    return x>=1 && x<=dx && y>=1 && y<=dy;
}

void dfsReplace(int x, int y){
    if(inRange(x,y) && graph[x][y]=='?')
        return ;
    for(int i=0;i<k;++i){
        int x2, y2;
        if(inRange(x,y)&&graph[x][y]=='#')
            x2=x-winds[i].first,
            y2=y-winds[i].second;
        else
            x2=x+winds[i].first,
            y2=y+winds[i].second;
        if(inRange(x2,y2)&&graph[x2][y2]=='?')
            graph[x2][y2]=graph[x][y],
            //'#'的上风位如果是'?'那这个位置一定是'#'，否则这个'#'会被认定为boundary并且这个'?'会被置为'.'，与当前状态矛盾
            // '.'的下风位若为‘?'那此处必然为’.‘，假如这个点是'#'，那它一定是boundary，不可能没被发现
            dfsReplace(x2,y2);
    }
}

int main(){
    cin>>dx>> dy>> k;
    graph.resize(dx+1, vector<char>(dy+1, '?'));
    winds.resize(k);
    for (int i = 0; i < k; ++i) {
        int b;
        cin>>winds[i].first>>winds[i].second>>b;
        for(int j=0;j<b;++j){
            int x,y;
            cin>>x>>y;
            graph[x][y]='#';
            int x2=x-winds[i].first, y2=y-winds[i].second;
            if(inRange(x2,y2))
                graph[x2][y2]='.';
        }
    }
    for(int x=-dx; x<=2*dx; ++x)
        for(int y=-dy; y<=2*dy; ++y){
            dfsReplace(x,y);
        }
    //输出结果，待定的'?'置为'.'是最小结构，置为'#'是最大结构
    for(int j=1;j<=dy;++j){
        for(int i=1;i<=dx;++i)
            cout<<(graph[i][j]=='?'?graph[i][j]:'.');
        cout<<endl;
    }
    for(int j=1;j<=dy;++j){
        for(int i=1;i<=dx;++i)
            cout<<(graph[i][j]=='?'?graph[i][j]:'#');
        cout<<endl;
    }
}