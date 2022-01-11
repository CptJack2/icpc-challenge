#include <vector>
#include "iostream"
#include "string"
#include "algorithm"
#include "set"
using namespace std;

int inf=1e8;
class Tarp{
public:
    int X1,X2,Y1,Y2;
    Tarp(int x1,int y1,int x2,int y2):X1(x1),Y1(y1),X2(x2),Y2(y2){}
    Tarp(){}
};
enum direction_enum{
    in=1,
    out=2,
};
struct allx_stru{
    int x,index;
    direction_enum direction;
    allx_stru(int x,int index,enum direction_enum dir): x(x), direction(dir), index(index){}
};
float cal_y(int x,int x1,int y1,int x2,int y2){
    return float(y1-y2)/(x1-x2)*(x-x1)+y1;
}
class dp_ele{
public:
    int x,delta;
    dp_ele(int x,int d):x(x),delta(d){}
};
auto deltas_cmp=[](const dp_ele& a,const dp_ele& b){return a.x<b.x;};
class dp_solver{
public:
    vector<dp_ele> deltas;
    int dp_L_actual,x_L,x_R;
    dp_solver(int L,int R): x_L(L), x_R(R){deltas.emplace_back(L, -inf);deltas.emplace_back(R + 1, inf);}
    void roll(int start,int end){
        if(start<end){
            int i= lower_bound(deltas.begin(),deltas.end(),dp_ele(start+1,0),deltas_cmp)-deltas.begin();
            if(i>=deltas.size())return;
            start=deltas[i].x;
            while(start<=end){
                if(deltas[i].delta>0) {
                    if (i + 1 < deltas.size() && deltas[i + 1].x <= end + 1)
                        deltas[i + 1].delta += deltas[i].delta;
                    else
                        deltas.insert(deltas.begin() + i + 1, dp_ele(end + 1, deltas[i].delta));
                    deltas.erase(deltas.begin()+i);
                } else{
                    i+=1;
                    if(i>= deltas.size())break;
                }
                start=deltas[i].x;
            }
        } else{
            int i= lower_bound(deltas.begin(),deltas.end(),dp_ele(start+1,0),deltas_cmp)-deltas.begin()-1;
            if(i<0)return;
            start=deltas[i].x;
            while(start>end){
                if(deltas[i].delta<0) {
                    if (i - 1 >= 0 && deltas[i - 1].x >= end) {
                        deltas[i - 1].delta += deltas[i].delta;
                        deltas.erase(deltas.begin() + i);
                    } else {
                        deltas.insert(deltas.begin() + i, dp_ele(end, deltas[i].delta));
                        deltas.erase(deltas.begin() + i + 1);
                    }
                }
                i-=1;
                if(i<0)break;
                start=deltas[i].x;
            }
        }
    }
    void add(int start, int end){
        auto update=[&](int x,int delta){
            int i= lower_bound(deltas.begin(),deltas.end(),dp_ele(x,0),deltas_cmp)-deltas.begin();
            if(i<deltas.size() && deltas[i].x==x)
                deltas[i].delta+=delta;
            else
                deltas.insert(deltas.begin()+i,dp_ele(x,delta));
        };
        if(start<end){
            update(start,1);
            update(end+1,-1);
        } else{
            update(start+1,1);
            update(end,-1);
        }
        if(start<=x_L<=end)
            dp_L_actual+=1;
    }
    int find_min(){
        int i= lower_bound(deltas.begin(),deltas.end(),dp_ele(x_L+1,0),deltas_cmp)-deltas.begin();
        int j= lower_bound(deltas.begin(),deltas.end(),dp_ele(x_R,0),deltas_cmp)-deltas.begin()-1;
        int actual,ret;
        actual=ret=dp_L_actual;
        for(int k=i;k<=j;k++){
            actual+=deltas[k].delta;
            ret=min(actual,ret);
        }
        return ret;
    }
};
int main(){
    int L,R,N;
    cin >> L >> R >> N;
    vector<Tarp> data(N+1);
    vector<allx_stru>allX;
    data[0]=Tarp(L,0,R,0);
    for(int i=1;i<=N;i++){
        int x1, x2, y1, y2;
        cin>>x1>>y1>>x2>>y2;
        data[i]=Tarp(x1,y1,x2,y2);
        allX.emplace_back(min(x1,x2),i,in);
        allX.emplace_back(max(x1,x2),i,out);
    }
    std::sort(allX.begin(), allX.end(), [] (const allx_stru& a, const allx_stru& b) { return a.x < b.x; });

    vector<vector<int>> dependency(N+1);
    vector<int>scan_set;
    for(auto xstru : allX){
        if(xstru.direction==in){
            vector<float> ys;
            for(auto s:scan_set)ys.push_back(cal_y(xstru.x,data[s].X1,data[s].Y1,data[s].X2,data[s].Y2));
            const Tarp& t=data[xstru.index];
            float h=cal_y(xstru.x,t.X1,t.Y1,t.X2,t.Y2);
            auto it=lower_bound(ys.begin(),ys.end(),h);
            int insert_pos=it-ys.begin();
            scan_set.insert(scan_set.begin()+insert_pos,xstru.index);
            if(insert_pos+1< scan_set.size())
                dependency[scan_set[insert_pos+1]].push_back(xstru.index);
            if(insert_pos-1>=0)
                dependency[xstru.index].push_back(scan_set[insert_pos-1]);
        } else{
            int index=0;
            for(auto s:scan_set)
                if(s==xstru.index)break;
                else ++index;
            if(index-1>=0 && index+1<scan_set.size())
                dependency[scan_set[index+1]].push_back(scan_set[index-1]);
            scan_set.erase(scan_set.begin()+index);
        }
    }

    vector<int> topo_sorted;
    set<int> sorted;
    while(topo_sorted.size()<dependency.size()-1){
        bool noAllMet= true;
        for(int i=1;i<dependency.size();i++){
            bool m= true;
            if(sorted.find(i)!=sorted.end())continue;
            for(auto dep : dependency[i])
                if(sorted.find(dep)==sorted.end()){m= false;break;}
            if(m){noAllMet= false; topo_sorted.push_back(i); sorted.insert(i);}
        }
        if(noAllMet)throw runtime_error("can't topology sort");
    }

    dp_solver solver(L,R);
    for(auto it=topo_sorted.rbegin();it!=topo_sorted.rend();it++){
        int x1,x2;
        x1=data[*it].X1;
        x2=data[*it].X2;
        solver.roll(x2,x1);
        solver.add(x2,x1+(x2<x1?-1:1));
    }
    int ans=solver.find_min();
    cout<<ans;
}