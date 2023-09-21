#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
int K, M;

vector<int> col;//M个col和各自的牌数量
double hands = 0.0, matched = 0.0;
vector<int> v;//控制分为K个牌分别属于M个类别中的哪一类

void rec(int i, int x) {
    if (i < K) {
        for (v[i] = x; v[i] < M; v[i]++) rec(i + 1, v[i]);
        return;
    }
    //算一个h和m，是总的hands和matched的一部分
    double h = 1.0, m = 0.0;
    vector<double> fact(K);
    //求从Mi中取K个的组合数
    for (int ii = 0, last = -1, n = 1; ii < K; ii++) {
        if (v[ii] == last) n++; else n = 1;
        last = v[ii];
        fact[ii] = double(col[v[ii]] - (n - 1)) / n;
        h *= fact[ii];
    }
    if (h == 0.0) return;
    for (int ii = 0; ii < K; ii++)
        if (ii == K - 1 || v[ii] != v[ii + 1]) {
            m += h / fact[ii];//C 52 4
        }
    for (int ii = 1; ii <= K - 1; ii++) m *= ii;//A 52 4
    hands += h;
    matched += min(h, m);
}

int main() {
//    cin >> K >> M;
//    K=5;M=1;    col=vector<int>{52};
//    K=3;M=1;    col=vector<int>{10};// A 10 2 / C 10 3
    K=2;M=2;    col=vector<int>{3,3};//
    K=3;M=2;    col=vector<int>{5,5};//
//    K=3;M=4;col=vector<int>{5,12,1,2};
    col.resize(M);
    v.resize(K);
//    for (int i = 0; i < M; i++) cin >> col[i];

    rec(0, 0);
    printf("%.12lf\n", matched / hands);
}