#include "bits/stdc++.h"

using namespace std;

int dx, dy, k;
vector<vector<char>> graph;
vector<pair<int,int>> winds;

inline bool inRange(int x, int y){
    return x>=1 && x<=dx && y>=1 && y<=dy;
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

}