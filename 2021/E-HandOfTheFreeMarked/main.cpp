#include "bits/stdc++.h"

using namespace std;

int K, M;

vector<int> markedNum;//M个种类各自的牌数量
double hands = 0.0, matched = 0.0;//全部可能的K组合数，K-1排列数
vector<int> cardType;//控制K个牌分别属于M个类别中的哪一类

void calaculateProbability(){
    double h = 1.0, m = 0.0;//使用long long会导致部分用例溢出。。。
    vector<double> fact(K);
    //从M类牌中各取cardType指定的数目，计算组合数
    for (int ii = 0, last = -1, n = 1; ii < K; ii++) {
        if (cardType[ii] == last)
            n++;
        else
            n = 1;
        last = cardType[ii];
        fact[ii] = double(markedNum[cardType[ii]] - (n - 1)) / n;
        h *= fact[ii];
    }
    if (h == 0.0) return;
    //计算取K-1张牌的排列数，每一类都少取一张，再乘一个K-1的全排列
    for (int ii = 0; ii < K; ii++)
        if (ii == K - 1 || cardType[ii] != cardType[ii + 1]) {
            m += h / fact[ii];
        }
    //乘K-1全排列
    for (int ii = 1; ii <= K - 1; ii++)
        m *= ii;
    hands += h;
    matched += min(h, m);
}

void generateCardType(){
    //决定K张牌分别属于M类中的哪一类
    while(1){
        calaculateProbability();
        int i=K-1;
        for(;i>=0;--i)
            if(cardType[i]<M-1)break;
        if(i<0)break;
        ++cardType[i];
        for(int j=i;j<K;++j)cardType[j]=cardType[i];
    }
}

int main() {
    cin >> K >> M;
    markedNum.resize(M);
    cardType.resize(K, 0);
    for (int i = 0; i < M; i++) cin >> markedNum[i];
    generateCardType();
    printf("%.12lf\n", matched/hands);
}