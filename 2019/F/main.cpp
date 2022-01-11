#include <vector>
#include "iostream"
#include "string"
#include "algorithm"
#include "set"
using namespace std;

int max_int=0x7FFFFFFF;
class Tarp{
public:
    int X1,X2,Y1,Y2;
    Tarp(int x1,int y1,int x2,int y2):X1(x1),Y1(y1),X2(x2),Y2(y2){}
};
enum direction{
    in=1,
    out=2,
};
struct allx_stru{
    int x,index;
    direction direction;
    allx_stru(int x,int index,enum direction dir):x(x),direction(dir),index(index){}
};
float cal_y(int x,int x1,int y1,int x2,int y2){
    return float(y1-y2)/(x1-x2)*(x-x1)+y1;
}
int main(){
    int L,R,N;
    cin >> L >> R >> N;
    vector<Tarp> data(N+1);
    vector<allx_stru>allX;
    data[0]=Tarp(L,0,R,0);
    allX.emplace_back(L,0,in);
    allX.emplace_back(R,0,out);
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
            int index= lower_bound(scan_set.begin(),scan_set.end(),xstru.index)-scan_set.begin();
            if(index-1>=0 && index+1<scan_set.size())
                dependency[scan_set[index+1]].push_back(scan_set[index-1]);
            scan_set.erase(scan_set.begin()+index);
        }
    }

    vector<int> topo_sorted(N+1);
    set<int> sorted;
    while(topo_sorted.size()<dependency.size()){
        bool noAllMet= true;
        for(int i=0;i<dependency.size();i++){
            bool m= true;
            if(sorted.find(i)!=sorted.end())continue;
            for(auto dep : dependency[i])
                if(sorted.find(dep)==sorted.end()){m= false;break;}
            if(m){noAllMet= false; topo_sorted.push_back(i); sorted.insert(i);}
        }
        if(noAllMet)throw exception("can't topology sort");
    }
}