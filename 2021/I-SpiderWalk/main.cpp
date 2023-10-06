#include "bits/stdc++.h"
using namespace std;

//逆时针顺拐
//stringstream cin(R"(
//11 2 6
//20 4
//10 3
//)");
//3 2 1 2 1 0 1 2 3 4 4

// 逆时针逆拐
//stringstream cin(R"(
//11 1 6
//20 3
//10 4
//)");
//4 3 2 1 1 0 1 2 3 4 5

//stringstream cin(R"(
//11 1 6
//20 11
//)");

int main() {
    int N, M, S, D, T, s, os;
    ::cin >> N >> M >> S;
    S--;
    s=S; os=s;
    vector<pair<int, int>> b;//bridges
    for (int i = 0; i < M; i++) {
        ::cin >> D >> T;
        b.push_back({-D, T - 1});
    }
    sort(b.begin(), b.end());//距离从大到小排序

    //邻近的两个strand答案只会相差±1或者0
    map<int, int> m;//strand index , 从key点逆时针离开的斜率
    m[S] = 1;//strand s逆时针往前斜率是1递增
    m[(S + N / 2) % N] = 0;//s逆时针转够半圈，最后如果是两个，数值一样，斜率是0
    m[(S + (N + 1) / 2) % N] = -1;//s顺时针转半圈，那一根开始递减，斜率是-1
    auto pred = [&](map<int, int>::iterator it) { return --(it == m.begin() ? m.end() : it); };
    auto succ = [&](map<int, int>::iterator it) {
        ++it;
        return (it == m.end() ? m.begin() : it);
    };
    auto getAll = [&](int x) {//当前和前后两个位置，逆时针离开的斜率
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
    auto swp = [&](int x) {//在x位置插入一条当前d最小的桥
        auto[pit, it, nit] = getAll(x);
        //当前、前、后位置，逆时针离开的斜率
        if (it->second == 0) return;//桥连接的两个strand桥数一样，无需做任何操作
        int xd = it->second, pd = pit->second, nd = nit->second;
        //一般情况,新修桥后,x和n位置互换,路程互换。对应的差数要修改,其他strand不受影响
        pd += xd;
        nd += xd;
        xd = -xd;
        //特殊情况1:原来路径从n建桥到x,从x建桥到p,有了桥后, n的路程可以-1,x到p的桥建在新桥前，路程不变
        if (pit->second == 1 && it->second==1) {
            xd=0;//n的路程-1,x不变,xd由1变为0
        }else
        //特殊情况2:原来路径从x建桥到n,从p建桥到x,有了新桥,从x往后斜率为-1的区间（包括p）的这一段都可以-1
        if (pit->second == -1 && it->second==-1) {
            set((pit->first + (N - 1)) % N, pred(pit)->second - 1);
        }
        //特殊情况3:原来路径从x建桥到n,从n建桥到nn,有了新桥,x路程-1； n到nn的桥建在新桥前，路程不变
        if (nit->second == -1 && it->second==-1) {
            xd=0;
        }else
        //特殊情况4:原来路径从nn建桥到n,从n建桥到x,nn往后斜率为1的区间(包括n)都可以-1
        if (nit->second == 1 && it->second==1) {
            set(succ(nit)->first, succ(nit)->second + 1);
        }
        set((x + N - 1) % N, pd);
        set(x, xd);
        set((x + 1) % N, nd);
    };

    for (auto[_, t] : b) {
        os=s;
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