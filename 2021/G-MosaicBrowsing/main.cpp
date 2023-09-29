#include "bits/stdc++.h"
using namespace std;

typedef bitset<1000> bs;
//class bs:public array<int,1000>{
//public:
//    void set(int ind){
//
//    }
//};


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
    int PY, PX, QY, QX, PC, QC;//motif和mosiac最大1000x1000
    ::cin >> PY >> PX;
    //g和cg是根据motif构造的pattern
    vector<bs> g(PX);//col用数组下标表示,row用bitset位表示, 有颜色grid
    vector<vector<bs>> cg(101, vector<bs>(PX));//color，每个颜色层grid，∑cg=~b
    auto showPat=[&](){
        cout<<"g"<<endl;
        for (int i = 0; i < PX; ++i) {
            for (int j = 0; j < PY; ++j) {
                cout<<g[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    };
    //通过motif构造pattern
    int mxC=0;
    for (int y = 0; y < PY; y++)//motif是PYxPX，g和cg是PXxPY
        for (int x = 0; x < PX; x++) {
            ::cin >> PC;
            mxC=max(mxC,PC);
            if (!PC) continue;//0是wildcard
            g[PX - 1 - x].set(PY - 1 - y);//转置，并且上下左右对称反转
            cg[PC][PX - 1 - x].set(PY - 1 - y);
//            showPat();
//            int a=1;
        }
    //motif颜色反转
    for (auto &v : cg) for (auto &b : v) b = ~b;

//    showPat();
    auto showCP=[&](int c){
//        for (int c = 0; c <mxC ; ++c){
            cout << "cg " <<c<< endl;
            for (int i = 0; i < PX; ++i) {
                for (int j = 0; j < PY; ++j) {
                    cout << cg[c][i][j]<<" ";
                }
                cout << endl;
            }
            cout << endl;
//        }
    };
//    showCP();
//    return 0;
    auto op=&decltype(showPat)::operator();
    auto scp=&decltype(showCP)::operator();
    ::cin >> QY >> QX;
    vector<bs> fail(QX);
    //根据mosiac构造的待匹配,都按颜色分层
    vector<vector<bs>> gg(101, vector<bs>(PX));
    vector<vector<int>> cookie(101, vector<int>(PX, -1));
    auto printBs=[&](const bs& bs1){
        auto str=bs1.to_string();
        reverse(str.begin(),str.end());
        //str=str.substr(0,QX);
        std::string result;
        for (size_t i = 0; i < str.length(); i++) {
            result += str[i];
            if (i != str.length() - 1) {
                result += ' ';
            }
        }
        return result;
    };
    for (int y = 0; y < QY; y++) {
        for (int x = 0; x < QX; x++) {
            ::cin >> QC;//读入mosaic一格颜色
            //根据颜色分层构建fail
//            auto t=max((PX - 1) - x, 0);
//            int a=1;
            for (int px = max((PX - 1) - x, 0); px < PX; px++) {
                int fx = x - (PX - 1) + px;
                if (fx > QX - PX) break;
//                printf("px %d QC %d y %d PY %d y<PY %s\n",px,QC,y ,PY,y<PY?"true":"false");
//                cout<<"g[px] "<<printBs(g[px])<<endl<<"cg[QC][px] "<<printBs(cg[QC][px])<<endl;
                if (cookie[QC][px] != y) {//这里每次比对motif的px列
                    cookie[QC][px] = y;
                    if (y < PY) {//g[px] & cg[QC][px] 有色且色不对,&出来那个位置才是1, motif的px列,每一格是否match当前这个QC
                        gg[QC][px] = (g[px] & cg[QC][px]) >> (PY - 1) - y;
                    } else {
                        gg[QC][px] = (g[px] & cg[QC][px]) << y - (PY - 1);
                    }
                }
//                for (int c = 1; c <=3 ; ++c) {
//                    cout << "gg " << c << endl;
//                    for (int i = 0; i < QY; ++i) {
//                        cout << printBs(gg[c][i]) <<endl;
//                    }
//                    cout << endl;
//                }
//                cout<<"gg[QC][px] "<<printBs(gg[QC][px])<<endl;
                //x - (PX - 1) + px = 0 x+px =PX-1=1 x=0,px=1 || x=1,px=0
                //所有gg[QC][px]的0位为0
                fail[x - (PX - 1) + px] |= gg[QC][px];
//                cout<<"x - (PX - 1) + px = "<<x - (PX - 1) + px<<endl;
//                cout<<"fail[x - (PX - 1) + px] "<<printBs(fail[x - (PX - 1) + px])<<endl;
//                cout<<endl;
                int a=1;
            }
        }
    }

    vector<pair<int, int>> v;
    //遍历fail,如果没有fail即是命中
    for (int y = 0; y <= QY - PY; y++)
        for (int x = 0; x <= QX - PX; x++)
            if (!fail[x][y]) {//fail[0][0]=0
                v.push_back({x, y});
            }
    cout << v.size() << endl;
    for (auto[x, y] : v) cout << y + 1 << ' ' << x + 1 << endl;
}