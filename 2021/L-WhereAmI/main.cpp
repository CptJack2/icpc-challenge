#include "bits/stdc++.h"

using namespace std;

vector<vector<char>> grid;
int dx,dy;

int main(){
    cin>>dx>>dy;
    grid.resize(dy+1);
    for(int y=dy; y >=1; --y) {
        grid[y].resize(dx+1);
        for (int x = 1; x <= dx; ++x)
            cin >>grid[y][x];
    }
    //生成螺旋距离矩阵
    vector<vector<int>> distance(201,vector<int>(201));
    for(int x=100,y=100,dx=0,dy=1,step=0,stepn=1,cur=0 ; x>=0&&x<201&&y>=0&&y<201 ; ++cur){
        distance[y][x]=cur;
        x+=dx;
        y+=dy;
        ++step;
        if(step==stepn){
            //切换方向
            swap(dx, dy);
            dy = -dy;
            step = 0;
            if(dy!=0)
                ++stepn;
        }
    }
    //对每个格子生成对应的X距离编码，按照X距离分类
    vector<vector<int>> gridDistances(40001); //下标是dist, 内容是格子序号(按先x后y排列, 1 -- dx*dy)
    for(int y=1;y<=dy;++y)
    for(int x=1;x<=dx;++x)
        if(grid[y][x]=='X')
            for(int y2=1;y2<=dy;++y2)
            for(int x2=1;x2<=dx;++x2){
                auto dist=distance[y-y2+100][x-x2+100];
                gridDistances[dist].push_back(x2+(y2-1)*dx);
            }
    //根据距离编码区分格子
    vector<int> gridCategory(dx*dy+1, 0), //每一个格子属于的类别，动态更新
        categorySteps(dx * dy, 0), //每一类格子的step数量
        categorySize = {dx*dy}; //每个类别的格子数
    for(int dist=0;categorySize.size()<dx*dy;++dist){
        auto& gridAtDist=gridDistances[dist];
        //按照category顺序排序
        sort(gridAtDist.begin(), gridAtDist.end(), [&](int a, int b){return gridCategory[a] < gridCategory[b];});
        for(int i=0,j=0; i < gridAtDist.size(); i=j){
            for(; j < gridAtDist.size() && gridCategory[gridAtDist[j]] == gridCategory[gridAtDist[i]]; ++j);
            int& sz=categorySize[gridCategory[gridAtDist[i]]];
            //当前comp category的所有格子编码都是一样的,靠这个X无法区分
            if (j - i == sz)
                continue;
            //当前X可以唯一区分一个格子，新建一类
            if (j - i == 1)
                categorySteps[categorySize.size()] = dist;
            sz-=j-i;
            if (sz == 1)
                categorySteps[gridCategory[gridAtDist[i]]] = dist;//当前X的位置是.的格子只有一个,它可以被区分
            //当前距离的category分裂开来，继续流程
            for (int k = i; k < j; k++) gridCategory[gridAtDist[k]] = categorySize.size();
            categorySize.push_back(j - i);
        }
    }
    //统计数据
    int mx = 0, tot = 0;
    for (int i = 0; i < dx * dy; i++) {
        mx = max(mx, categorySteps[i]);
        tot += categorySteps[i];
    }

    cout << fixed << setprecision(9) << double(tot) / dx / dy << endl;
    cout << mx << endl;
    bool first = true;
    for (int i = 1; i <= dx * dy; i++)
        if (categorySteps[gridCategory[i]] == mx) {
            if (!first) cout << ' ';
            first = false;
            cout << '(' << (i-1) % dx + 1 << ',' << (i-1) / dx + 1 << ')';
        }
    cout << endl;
}



















