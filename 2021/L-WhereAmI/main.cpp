#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    int X, Y;
    cin >> X >> Y;
    vector<string> g(Y);
    vector<pair<int,int>> XCords;
    int numX=0;
    for (int y = Y - 1; y >= 0; y--) {
        cin >> g[y];
        for(int x=0;x<X;++x)if(g[y][x]=='X'){
            XCords.emplace_back(x,y);
            ++numX;
        }
    }

    vector<vector<int>> dist(201, vector<int>(201));//螺旋矩阵,对应距离
    auto printDist=[&](){
        for (int j = 111; j>= 90 ; --j){
            for(int i=90;i<111;++i)
                cout<<dist[j][i]<<" ";
            cout<<endl;
        }
//        fflush(stdout);
    };
    auto printDist_operator_call=&decltype(printDist)::operator();//in gdb: call (printDist.*printDist_operator_call)()
    //生成螺旋矩阵
    for (int x = 100, y = 100, dx = 0, dy = 1, step = 0, stepn = 1, cur = 0; y < 201; cur++) {
        dist[y][x] = cur;
        x += dx;
        y += dy;
        if (++step == stepn) {
            //切换方向
            swap(dx, dy);
            dy = -dy;
            step = 0;
            if (dy) stepn++;
        }
    }
//    auto calcDist=[&](int x, int y){
//        vector<int> res(numX); int c=0;
//        for(auto [x1,y1]:XCords){
//            res[c++]=dist[y1-y+100][x1-x+100];
//        }
//        sort(res.begin(),res.end());
//        return res;
//    };
//    auto op=&decltype(calcDist)::operator();//in gdb: call (printDist.*printDist_operator_call)()
    vector<vector<vector<int>>> dists(Y, vector<vector<int>>(X, vector<int>(numX)));
//    for (int j = 0; j < X; ++j) {
//        for (int i = 0; i < Y; ++i) {
//            dists[j][i]=calcDist(j,i);
//        }
//    }
//    printDist();
    //通过dist生成obs
    vector<vector<int>> obs(40001);//下标是dist, 内容是格子序号, < X*Y
    for (int y = 0; y < Y; y++)
        for (int x = 0; x < X; x++)
            if (g[y][x] == 'X')
                for (int sy = 0, i = 0; sy < Y; sy++)
                    for (int sx = 0; sx < X; sx++, i++) {//找出通过X距离编码, 每个格子对应的每个X的dist
                        obs[dist[y - sy + 100][x - sx + 100]].push_back(i);
                    }
    //obs到comp
    //compt每一个格子的前缀长度, comp从格子坐标映射到compt下标, compsz不断分裂
    vector<int> comp(X * Y), compt(X * Y), compsz{X * Y};
    for (int t = 0; compsz.size() < X * Y; t++)//逐渐增大dist,直至所有X*Y个格都能区分
        if (obs[t].size()) {
            vector<int> &v = obs[t];//编码中所有存在距离t的格子
            sort(v.begin(), v.end(),
                 [&](int x, int y) { return comp[x] < comp[y]; });//根据comp排一下序
            for (int i = 0, j = 0; i < v.size(); i = j) {
                for (j++; j < v.size() && comp[v[j]] == comp[v[i]]; j++);
                int &sz = compsz[comp[v[i]]];
                if (j - i == sz) continue;//当前comp category的所有格子编码都是一样的,靠这个X无法区分
                if (j - i == 1)
                    compt[compsz.size()] = t;//当前X可以唯一区分一个格子
                sz -= j - i;
                if (sz == 1)
                    compt[comp[v[i]]] = t;//当前X的位置是.的格子只有一个,它可以被区分
                for (int k = i; k < j; k++) comp[v[k]] = compsz.size();
                compsz.push_back(j - i);
            }
        }

    int mx = 0, tot = 0;
    for (int i = 0; i < X * Y; i++) {
        mx = max(mx, compt[i]);
        tot += compt[i];
    }

    cout << fixed << setprecision(9) << double(tot) / X / Y << endl;
    cout << mx << endl;
    bool first = true;
    for (int i = 0; i < X * Y; i++)
        if (compt[comp[i]] == mx) {
            if (!first) cout << ' ';
            first = false;
            cout << '(' << i % X + 1 << ',' << i / X + 1 << ')';
        }
    cout << endl;
}