#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
    int N, M, S, D, T;
    cin >> N >> M >> S;
    S--;
    vector<pair<int, int>> b;//bridges
    for (int i = 0; i < M; i++) {
        cin >> D >> T;
        b.push_back({-D, T - 1});
    }
    sort(b.begin(), b.end());//距离从大到小排序

    //邻近的两个strand答案只会相差±1或者0
    map<int, int> m;//strand index to 压缩斜率
    m[S] = 1;//strand s逆时针往前斜率是1递增
    m[(S + N / 2) % N] = 0;//s逆时针转够半圈，最后如果是两个，数值一样，斜率是0
    m[(S + (N + 1) / 2) % N] = -1;//s顺时针转半圈，那一根开始递减，斜率是-1
    auto pred = [&](map<int, int>::iterator it) { return --(it == m.begin() ? m.end() : it); };
    auto succ = [&](map<int, int>::iterator it) {
        ++it;
        return (it == m.end() ? m.begin() : it);
    };
    auto getAll = [&](int x) {//当前和前后两个位置，逆时针进入的斜率
        auto it = pred(m.upper_bound(x)), pit = it, nit = succ(it);
        if (pit->first == x) pit = pred(pit);
        if (nit->first != (x + 1) % N) nit = it;
        return make_tuple(pit, it, nit);
    };
    auto set = [&](int x, int d) {
        auto[pit, it, nit] = getAll(x);
        int xd = it->second, pd = pit->second, nd = nit->second;
        if (xd == d) return;
        if (d == pd)
            m.erase(x);
        else
            m[x] = d;
        if (nd == d)
            m.erase((x + 1) % N);
        else
            m[(x + 1) % N] = nd;
    };
    auto swp = [&](int x) {
        auto[pit, it, nit] = getAll(x);
        int xd = it->second, pd = pit->second, nd = nit->second;
        if (xd == 0) return;
        xd = -xd;
        pd -= xd;
        nd -= xd;
        if (pd == 2) {
            pd--;
            xd++;
        }else if (pd == -2) {
            pd++;
            set((pit->first + (N - 1)) % N, pred(pit)->second - 1);
        }
        if (nd == -2) {
            nd++;
            xd--;
        }else if (nd == 2) {
            nd--;
            set(succ(nit)->first, succ(nit)->second + 1);
        }
        set((x + N - 1) % N, pd);
        set(x, xd);
        set((x + 1) % N, nd);
    };

    int s = S;
    for (auto[_, t] : b) {
        if (s == t) s = (s + 1) % N; else if (s == (t + 1) % N) s = t;
        swp(t);
    }

    vector<int> ret(N);
    for (int i = 0, cur = 0, d = 0; i < N; i++) {
        ret[s] = cur;
        if (m.count(s)) d = m[s];
        cur += d;
        s = (s + 1) % N;
    }
    for (auto x : ret) cout << x << endl;
}