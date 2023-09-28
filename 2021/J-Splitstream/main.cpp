#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int M, N, Q;
    cin >> M >> N >> Q;
    vector<vector<int>> nd(1);//nodes, store 2 input and 2 output
    int mx = 0;
    for (int i = 1; i <= N; i++) {
        char ch;
        int x, y, z;
        cin >> ch >> x >> y >> z;
        mx = max<int>({mx, x, y, z});
        if (ch == 'S') nd.push_back({x, 0, y, z}); else nd.push_back({x, y, z, 0});
    }
    vector<int> oin(mx + 1), oout(mx + 1);//input/output stream num to node num
    for (int i = 1; i <= N; i++) oin[nd[i][0]] = oin[nd[i][1]] = oout[nd[i][2]] = oout[nd[i][3]] = i;

    vector<int> osz(mx + 1, -1);//stream size
    osz[0] = 0;
    function<void(int, int)> rec = [&](int x, int sz) {//stream num and size
        osz[x] = sz;
        if (oin[x] == 0) return;//这个stream没有input到node了
        auto const &v = nd[oin[x]];//x输入到的node
        if (osz[v[0]] == -1 || osz[v[1]] == -1) return;//v的任意一个输入是空stream
        if (v[1]) {//v是merge node
            rec(v[2], osz[v[0]] + osz[v[1]]);
        } else {//split node
            rec(v[2], (osz[v[0]] + 1) / 2);
            rec(v[3], (osz[v[0]]) / 2);
        }
    };
    rec(1, M);
//    for (int i = 2; i <= mx; i++)//多余
//        if (!oout[i])//为什么会有没有从node输出的stream?
//            rec(i, 0);

    for (int q = 0; q < Q; q++) {
        int x, k;
        cin >> x >> k;
        if (k > osz[x]) {
            cout << "none" << endl;
            continue;
        }
        while (x != 1) {//模拟流向回溯计算
            auto const &v = nd[oout[x]];
            if (v[1]) {//merge node
                int sz = min(osz[v[0]], osz[v[1]]);
                if (k <= 2 * sz) {
                    x = v[!(k % 2)];
                    k = (k + 1) / 2;
                } else {
                    x = v[osz[v[1]] > osz[v[0]]];
                    k -= sz;
                }
            } else {//split node
                k = 2 * k - (v[2] == x);
                x = v[0];
            }
        }
        cout << k << endl;
    }
}