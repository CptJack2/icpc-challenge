//from http://littledva.cn/article-33/
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include "iostream"
using namespace std;
using pii=pair<int, int>;
stringstream ss("10 20 5\n"
                "32 50 12 60\n"
                "30 60 8 70\n"
                "25 70 0 80\n"
                "15 30 28 40\n"
                "5 20 14 25");
inline int read(){
    int res;
    cin>>res;
    return res;
}
int main(){
    int ll=read(), rr=read(), N=read();
    vector<pair<pii, pii>> Seg(N+1);
    vector<pii> X(N+N+1);
    Seg[0]={{0, -1000}, {1e9+1, -1000}};
    for(int i=1; i<=N; i++){
        int a=read(), b=read(), c=read(), d=read();
        Seg[i]={{a,b},{c,d}};
        if(a>c)swap(Seg[i].first, Seg[i].second);
        //X[i].second > 0 代表是线段左端点, <0代表右端点
        X[i]=make_pair(Seg[i].first.first, i);
        X[i+N]=make_pair(Seg[i].second.first, -i);
    }
    sort(X.begin(), X.end());
    int nowx;
    //算x在第i号直线上的位置y
    auto point = [&](int i, int x){
        return Seg[i].first.second + ((double)(x-Seg[i].first.first))/(Seg[i].second.first-Seg[i].first.first)*(Seg[i].second.second-Seg[i].first.second);
        //li: y1+ki*(x-x1)
    };
    //算直线a和b,在nowx上, 哪个y高
    auto cmp_Set = [&](const int a, const int b){
        auto x = point(a, nowx), y = point(b, nowx);
        return x < y;
    };
    set<int, decltype(cmp_Set)> Set(cmp_Set);
    Set.insert(0);
    vector<int> to(N+1), tail(N+1);
    for(int i=0; i<=N; i++){
        to[i]=tail[i]=i;
    }
    for(int _=1; _<=N+N; _++){
        int i = X[_].second;
        nowx = X[_].first;
        //从x轴左往右扫描, 遇到左端点插入到有序set中
        if(i>0) Set.insert(i);
        else{
            i=-i;
            auto p = Set.find(i);
            int j = *--p;
            to[tail[j]]=i;
            tail[j]=tail[i];
            Set.erase(i);
        }
    }
    int a=1;
    for(int x=1; x<=N+N; x++){
        int i = X[x].second;
        if(i>0)
            Seg[i].first.first=x<<1;
        else
            Seg[-i].second.first=x<<1;
    }
    vector<pair<pii, bool>> Ban(N+1);
    for(int i=to[0], j=N; j; i=to[i], j--){
        Ban[j]={{Seg[i].first.first, Seg[i].second.first}, Seg[i].first.second>Seg[i].second.second};
    }
    int inf=0x3f3f3f3f, l, r;
    if(ll<X[1].first || rr>X[X.size()-1].first)return puts("0"), 0;
    auto rightL = lower_bound(X.begin()+1, X.end(), make_pair(ll, -inf));
    l = (rightL - X.begin())*2 - (rightL->first != ll);
    auto leftR = lower_bound(X.begin()+1, X.end(), make_pair(rr+1, -inf)) -1;
    r = (leftR - X.begin())*2 + (leftR->first != rr);
    vector<int> dp(N*4+3, 0);
    set<int> Neg, Pos;
    dp[0]=inf;
    dp[l]=-inf;
    dp[r+1]=inf;
    Pos.insert(r+1);
    Neg.insert(l);
    for(int i=1; i<=N; i++){
        int L = Ban[i].first.first, R=Ban[i].first.second;
        if(Ban[i].second){
            auto i=Pos.lower_bound(L+1);
            while(i!=Pos.end() && *i<=R){
                auto j=i; j++;
                auto k=Neg.lower_bound(*i+1);
                int pos = R+1;
                if(j!=Pos.end() && *j<pos)pos=*j;
                if(k!=Neg.end() && *k<pos)pos=*k;
                dp[pos]+=dp[*i];
                dp[*i]=0;
                Pos.erase(i);
                if(dp[pos]>0)Pos.insert(pos), Neg.erase(pos);
                else if(dp[pos]<0)Neg.insert(pos), Pos.erase(pos);
                else Neg.erase(pos), Pos.erase(pos);
                i=Pos.lower_bound(L+1);
            }
            dp[L]++;
            if(dp[L]==0)Neg.erase(L);
            else if(dp[L]==1)Pos.insert(L);
            dp[R]--;
            if(dp[R]==0)Pos.erase(R);
            else if(dp[R]==-1)Neg.insert(R);
        }
        else{
            auto i=Neg.lower_bound(R+1);
            while(i!=Neg.begin() && *--i>L){
                auto j=i;
                auto k=Pos.lower_bound(*i+1);
                int pos = L;
                if(j!=Neg.begin() && *--j>pos)pos=*j;
                if(k!=Pos.begin() && *--k>pos)pos=*k;
                dp[pos]+=dp[*i];
                dp[*i]=0;
                Neg.erase(i);
                if(dp[pos]>0)Pos.insert(pos), Neg.erase(pos);
                else if(dp[pos]<0)Neg.insert(pos), Pos.erase(pos);
                else Neg.erase(pos), Pos.erase(pos);
                i=Neg.lower_bound(R+1);
            }
            dp[L+1]++;
            if(dp[L+1]==0)Neg.erase(L+1);
            else if(dp[L+1]==1)Pos.insert(L+1);
            dp[R+1]--;
            if(dp[R+1]==0)Pos.erase(R+1);
            else if(dp[R+1]==-1)Neg.insert(R+1);
        }
    }
    for(int i=1; i<=N*4; i++){
        dp[i]+=dp[i-1];
    }
    int ans = N;
    for(int i=l; i<=r; i++){
        ans = min(ans, dp[i]);
    }
    printf("%d\n", ans);
}