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
//    std::ios_base::sync_with_stdio(false);
//    std::cin.tie(nullptr);
    int rp, cp, rq, cq, color;//motif和mosiac最大1000x1000
    ::cin >> rp >> cp;
    //g和cg是根据motif构造的pattern
    vector<bs> motifColorOrWild(cp);//col用数组下标表示,row用bitset位表示, 有颜色grid
    vector<vector<bs>> motifColorLayers(101, vector<bs>(cp));//color，每个颜色层grid，∑cg=~b
    int mxC=0;
    for (int y = 0; y < rp; y++)//motif是PYxPX，g和cg是PXxPY
        for (int x = 0; x < cp; x++) {
            ::cin >> color;
            mxC=max(mxC, color);
            if (!color) continue;//0是wildcard
            //转置，并且上下左右对称反转. 转置是因为逐行读入mosiac，每读入一个色格，应用motif的一列去匹配mosiac一列的部分，转置后motif的一列放入一个bitset中。反转是因为要通过当前列的判断结果，回溯到前面的起始位置的fail结果。
            motifColorOrWild[cp - 1 - x].set(rp - 1 - y);
            motifColorLayers[color][cp - 1 - x].set(rp - 1 - y);
        }
    //motif颜色反转
    for (auto &v : motifColorLayers)
        for (auto &b : v)
            b = ~b;
    ::cin >> rq >> cq;
    vector<bs> fail(cq);
    //根据mosiac构造的待匹配,都按颜色分层
    vector<vector<bs>> matchResult(101, vector<bs>(cp));
    vector<vector<int>> colorMet(101, vector<int>(cp, -1));//在当前行的前面是否读入过相同颜色，减少bitset&的次数
    for (int y = 0; y < rq; y++) {
        for (int x = 0; x < cq; x++) {
            ::cin >> color;//读入mosaic一格颜色
            //根据颜色分层构建fail
            for (int px = max((cp - 1) - x, 0); px < cp; px++) {
                int fx = x - (cp - 1) + px;
                if (fx > cq - cp) break;//motif是否超出mosiac右边界
                if (colorMet[color][px] != y) {//这里每次比对motif的px列
                    colorMet[color][px] = y;
                    if (y < rp) {//g[px] & cg[QC][px] 有色且色不对,&出来那个位置才是1, motif的px列,每一格是否match当前这个QC
                        matchResult[color][px] = (motifColorOrWild[px] & motifColorLayers[color][px]) >> (rp - 1) - y;//左右移,去匹配mosiac对应位置
                    } else {
                        matchResult[color][px] = (motifColorOrWild[px] & motifColorLayers[color][px]) << y - (rp - 1);
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