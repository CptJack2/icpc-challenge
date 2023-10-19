#include "bits/stdc++.h"
using namespace std;

typedef bitset<1000> bs;

//stringstream cin(R"(2 1
//1
//2
//3 4
//1 2 1 2
//3 1 2 1
//1 2 1 2)");
int main() {
    int rp, cp, rq, cq, color;//motif和mosiac最大1000x1000
    ::cin >> rp >> cp;
    //g和cg是根据motif构造的pattern
    vector<bs> motifMask(cp);
    vector<vector<bs>> motifColorLayers(101, vector<bs>(cp));
    int mxC=0;
    for (int y = 0; y < rp; y++)
        for (int x = 0; x < cp; x++) {
            ::cin >> color;
            mxC=max(mxC, color);
            //0是wildcard
            if (color==0)
                continue;
            //转置（其实是为了按列存储），并且上下左右对称反转. 转置是因为逐行读入mosiac，每读入一个色格，应用motif的一列去匹配mosiac一列的部分，转置后motif的一列放入一个bitset中。反转是因为要通过当前列的判断结果，回溯到前面的起始位置的fail结果。
            motifMask[cp - 1 - x].set(rp - 1 - y);
            motifColorLayers[color][cp - 1 - x].set(rp - 1 - y);
        }
    ::cin >> rq >> cq;
    vector<bs> fail(cq);
    //根据mosiac构造的待匹配,都按颜色分层
    vector<vector<bs>> matchResult(101, vector<bs>(cp));
    vector<vector<int>> colCache(101, vector<int>(cp, -1));//在当前行的前面是否读入过相同颜色，减少bitset&的次数
    for (int y = 0; y < rq; y++) {
        for (int x = 0; x < cq; x++) {
            ::cin >> color;//读入mosaic一格颜色
            //根据颜色分层构建fail
            for (int px = max((cp - 1) - x, 0); px < cp; px++) {
                int fx = x - (cp - 1) + px;
                if (fx > cq - cp) break;//motif是否超出mosiac右边界
                if (colCache[color][px] != y) {//这里每次比对motif的px列
                    colCache[color][px] = y;
                    //匹配当前color的1位置都是能作为起始点的，0位置都是不满足的，fail矩阵语义相反，需要取反；和motifColorOrWild&后（避免取反后超出motif边界的1干扰）
                    auto rev= ~motifColorLayers[color][px] & motifMask[px];
                    //左右移,去匹配mosiac对应位置
                    if (y < rp) {
                        matchResult[color][px]=rev>> (rp - 1) - y;//左右移补的都是0,fail语义更容易实现
                    } else {
                        matchResult[color][px]=rev<< y - (rp - 1);
                    }
                }
                fail[x - (cp - 1) + px] |= matchResult[color][px];
            }
        }
    }

    vector<pair<int, int>> v;
    //遍历fail,如果没有fail即是命中
    for (int y = 0; y <= rq - rp; y++)
        for (int x = 0; x <= cq - cp; x++)
            if (!fail[x][y]) {//fail[0][0]=0
                v.push_back({x, y});
            }
    cout << v.size() << endl;
    for (auto[x, y] : v) cout << y + 1 << ' ' << x + 1 << endl;
}