#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
int K, M;

vector<int> col;
double hands = 0.0, matched = 0.0;
vector<int> v;

void rec(int i, int x) {
    if (i < K) {
        for (v[i] = x; v[i] < M; v[i]++) rec(i + 1, v[i]);
        return;
    }
    double h = 1.0, m = 0.0;
    vector<double> fact(K);
    for (int ii = 0, last = -1, n = 1; ii < K; ii++) {
        if (v[ii] == last) n++; else n = 1;
        last = v[ii];
        fact[ii] = double(col[v[ii]] - (n - 1)) / n;
        h *= fact[ii];
    }
    if (h == 0.0) return;
    for (int ii = 0; ii < K; ii++)
        if (ii == K - 1 || v[ii] != v[ii + 1]) {
            m += h / fact[ii];
        }
    for (int ii = 1; ii <= K - 1; ii++) m *= ii;
    hands += h;
    matched += min(h, m);
}

int main() {
    cin >> K >> M;
//    K=4;M=1;
    col.resize(M);
    v.resize(K);
    for (int i = 0; i < M; i++) cin >> col[i];
//    col=vector<int>{28};

    rec(0, 0);
    printf("%.12lf\n", matched / hands);
}