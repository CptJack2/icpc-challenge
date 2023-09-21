#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
int K, M;

vector<int> markedNum;//M个col和各自的牌数量
double hands = 0.0, matched = 0.0;
long long totalCombinations = 0, totalArrangements = 0;
vector<int> cardType;//控制分为K个牌分别属于M个类别中的哪一类

void calaculateProbability(){
    double h = 1.0, m = 0.0;
    vector<double> fact(K);
    //求从Mi中取K个的组合数
    for (int ii = 0, last = -1, n = 1; ii < K; ii++) {
        if (cardType[ii] == last) n++; else n = 1;
        last = cardType[ii];
        fact[ii] = double(markedNum[cardType[ii]] - (n - 1)) / n;
        h *= fact[ii];
    }
    if (h == 0.0) return;
    for (int ii = 0; ii < K; ii++)
        if (ii == K - 1 || cardType[ii] != cardType[ii + 1]) {
            m += h / fact[ii];//C 52 4
        }
    for (int ii = 1; ii <= K - 1; ii++) m *= ii;//A 52 4
    hands += h;
    matched += min(h, m);
}

void generateCardType(){
    for (int i = 0; i < K; ++i) {
        for (cardType[i] = 0; cardType[i] < M; cardType[i]++) {
            if(i>=1 && cardType[i]<cardType[i-1])continue;
            calaculateProbability();
        }
    }
}
void rec(int i, int x) {
    if (i < K) {
        for (cardType[i] = x; cardType[i] < M; cardType[i]++) rec(i + 1, cardType[i]);
        return;
    }
    //算一个h和m，是总的hands和matched的一部分
    double h = 1.0, m = 0.0;
    vector<double> fact(K);
    //求从Mi中取K个的组合数
    for (int ii = 0, last = -1, n = 1; ii < K; ii++) {
        if (cardType[ii] == last) n++; else n = 1;
        last = cardType[ii];
        fact[ii] = double(markedNum[cardType[ii]] - (n - 1)) / n;
        h *= fact[ii];
    }
    if (h == 0.0) return;
    for (int ii = 0; ii < K; ii++)
        if (ii == K - 1 || cardType[ii] != cardType[ii + 1]) {
            m += h / fact[ii];//C 52 4
        }
    for (int ii = 1; ii <= K - 1; ii++) m *= ii;//A 52 4
    hands += h;
    matched += min(h, m);
}

int main() {
//    K=5;M=1;    col=vector<int>{52};
//    K=3;M=1;    col=vector<int>{10};// A 10 2 / C 10 3
//    K=2;M=2; markedNum=vector<int>{3, 3};//
//    K=3;M=2; markedNum=vector<int>{5, 5};//
//    K=3;M=4;col=vector<int>{5,12,1,2};

    cin >> K >> M;
    for (int i = 0; i < M; i++) cin >> markedNum[i];

    markedNum.resize(M);
    cardType.resize(K, 0);

    rec(0, 0);
//    generateCardType();
    printf("%.12lf\n", matched / hands);
}