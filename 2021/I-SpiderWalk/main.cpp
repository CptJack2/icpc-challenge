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
        int xd = it->second, pd = pit->second, nd = nit->second;
        if (it->second == 0)//桥连接的两个strand桥数一样，无需做任何操作
            return;
        else if(it->second==1){//原来路径从n建桥到x, 有了桥后, n的路程可以-1
            if(pit->second==1){//原来路径要建桥从x到p,在新桥前建桥从x到p， x路程不变
                xd=0;//n直接可以从新桥到x,n路程-1,x不变,所以xd变0
            }else{//(-1)原来从p到x，加桥后x要加一桥从n绕回，路程+1，p不变 ;(0)原路径从x到p(因为n到x)
                //原来要用到前面添加的桥
                //                assert(pit->second!=-1 || x==os);
                if(pit->second==-1)
                    asm("int $0x3");
                xd=-it->second;//(-1) s从x变n, xd从1变-1 ; (0)x要在新桥前建桥抵消,路程+1, n-1, x+1 ,xd从1变-1
                ++pd;//(-1) s从x变n, pd -1变0; (0) x路程+1, pd 0变1
            }
            if(nit->second==1){//原路径从nn到n
                set(succ(nit)->first, succ(nit)->second + 1);//nn后面一段都可以-1
            }else{//(-1) 原路径从n经nn和n经x路程一样, 多了桥后n路程-1变成和nn一样; (0) 原来从nn和n路程相同, 有桥后n比nn少1
                ++nd;
            }
        }else if(it->second==-1){//原来路径从x到n, 有了桥后, x的路程可以-1
            if(pit->second==-1){//原来路径从p到x, 并且有若干个都经过p往x走
                set((pit->first + (N - 1)) % N, pred(pit)->second - 1);//逆时针找到这一段的头部,这一整段的桥数都可以-1
            }else{//原来路径从x到p,x到n距离都一样(+1)或者p和x已有桥相连(0). 加入桥后x路程-1: (1)x路程变得和p一样, pd从1变0; (0)p应在新桥前建桥往x. pd从0变-1. 所以pd -1
                --pd;
            }
            if(nit->second==-1){//原来的路径是从x到n,n到n的下一个
                xd=0;//n到nn的桥改为在新桥前面建,继续走老路,路程不变; x可以沿此桥到n, 路程-1
            }else{//原来路径从nn到n(1, n是当前的s, 加桥后x变成新的s)或者n到nn(x到n)有桥相连(0),
//                assert(nit->second!=1 || os==(x+1)%N);
                xd=-it->second;//x路程-1,(1)n路程从0变1,x从1变0; (0)n在新桥前要建桥抵消这个桥,路程+1; (0) 所以xd从-1变1
                --nd;//(1)nn不变,直连到n所以nd从-1变为0; (0)n路程增加了,nn不变,所以nd从0变1
            }
        }
        set((x + N - 1) % N, pd);//上一个位置新”离开“斜率，即x位置进入斜率
        set(x, xd);//当前位置新离开斜率
        set((x + 1) % N, nd);//下一个位置新离开斜率
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