#include "bits/stdc++.h"
using namespace std;

int64_t searchDirectionX = 1, searchDirectionY = 1; //搜索方向向量
int64_t ans=0;//求出的最大欧式距离

struct Point {
    int64_t x=0, y=0;
    Point operator-() const { return {-x, -y}; }
    Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
    Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
    Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
    //通过在searchDirection上的投影长度（点积）来给点排序，以找到searchDirection上最远的两点
    bool operator<(const Point& p) const { return x*searchDirectionX + y*searchDirectionY < p.x*searchDirectionX + p.y*searchDirectionY; }
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    Point ortho() const { return {-y, x}; }
    int64_t lensqr() const { return x*x+y*y; }
};


vector<vector<int>> tournamentTree;
vector<Point> memes;

//返回searchDirection上极小和极大的两点
pair<Point,Point> dfsTournamentTreeGetMinMax(int p){
    if(tournamentTree[p].empty())
        return {memes[p],memes[p]};
    Point minTotal, maxTotal;
    Point minDiff, maxDiff;
    bool diffInited=false;
    for(auto ch:tournamentTree[p]){
        auto [mn,mx]= dfsTournamentTreeGetMinMax(ch);
        minTotal+=mn;
        maxTotal+=mx;
        if(!diffInited){
            minDiff=mx+mn;
            maxDiff=mx+mn;
            diffInited=true;
        }else{
            minDiff=min(minDiff,mx+mn);
            maxDiff=max(maxDiff,mx+mn);
        }
    }
    return {-maxTotal+minDiff, -minTotal+maxDiff};
}

pair<Point,Point> tryDirection(Point dir){
    searchDirectionX =dir.x;
    searchDirectionY =dir.y;
    auto [mn,mx]= dfsTournamentTreeGetMinMax(1);
    ans=max(ans,mx.lensqr());
    ans=max(ans,mn.lensqr());
    return {mn,mx};
}

void traceHull(Point a,Point b){
//    if(a==b)return;
    auto [_,c]= tryDirection((b-a).ortho());
    if(a<c){
        traceHull(a,c);
        traceHull(c,b);
    }
}

int main(){
    int n;
    cin>>n;
    tournamentTree.resize(n+1);
    memes.resize(n+1);
    for(int i=1;i<=n;++i){
        int k,x,y;
        cin>>k;
        if(k==0){
            cin>>memes[i].x>>memes[i].y;
        }else{
            int j;
            for(int i2=0; i2 < k; ++i2) {
                cin>>j;
                tournamentTree[i].push_back(j);
            }
        }
    }
    auto [a,b]= tryDirection({1,1});
    traceHull(a,b);
    traceHull(b,a);
    cout<<ans<<endl;
}