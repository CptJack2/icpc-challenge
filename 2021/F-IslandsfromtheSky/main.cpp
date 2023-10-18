#include "bits/stdc++.h"

using namespace std;

const double pi=2* asin(1);

int n,m;

struct pnt{
    double x,y;
    pnt operator-(const pnt& p) const{return pnt{x-p.x,y-p.y};}
    pnt operator-() const{return pnt{-x,-y};}
    pnt operator+(const pnt& p) const{return pnt{x+p.x,y+p.y};}
    pnt perpendicular(){return pnt{-y,x};}
    pnt operator/(double k) const{return pnt{x/k,y/k};}
    pnt operator*(double k) const{return pnt{x*k,y*k};}
    double len() const{return hypot(x,y);}
};

double crossProduct(const pnt& a,const pnt& b){
    return a.x*b.y-a.y*b.x;
}

pnt pinf{2*1e6,2*1e6};//坐标绝对值<10^6

//通过p向任意方向做一无穷远射线穿过梯形的次数判断p在梯形内还是外，在梯形边上也返回true
bool pntInTrapz(const vector<pnt>& trapz, pnt p){
    int intersectCnt=0;
    for(int i=0;i<4;++i){
        auto p1=trapz[i], p2=trapz[i+1%4];
        auto cp1=crossProduct(p2-p1,p-p1),cp2=crossProduct(p2-p1,pinf-p1),
            cp3=crossProduct(pinf-p,p1-p),cp4=crossProduct(pinf-p,p2-p);
        //通过叉积是否同号判断p点向无穷远的射线是否穿过梯形边
        if(cp1>0&&cp2>0 || cp1<0&&cp2<0 || cp3>0&&cp4>0 || cp3<0&&cp4<0)
            continue;
        ++intersectCnt;
    }
    //如果射线穿过奇数次，p在梯形内，否则在外
    return intersectCnt%2==1;
}

int main(){
    cin>>n>>m;
    vector<vector<pnt>> islands(n);
    for(int i=0;i<n;++i){
        int ni;
        cin>>ni;
        islands[i].resize(ni);
        for(int j=0;j<ni;++j)
            cin>>islands[i][j].x>>islands[i][j].y;
    }
    vector<pair<pnt,pnt>>flights(m);
    vector<pair<int,int>>fheights(m);
    for(int i=0;i<m;++i){
        cin>>flights[i].first.x>>flights[i].first.y>>fheights[i].first;
        cin>>flights[i].second.x>>flights[i].second.y>>fheights[i].second;
    }
    double lo=0, hi=pi/2;// todo long double
    //27次二分后精度就达到要求
    for(int rep=0;rep<27;++rep){
        double theta=(lo+hi)/2;
        vector<bool> covered(n,false);
        //m条航线根据当前θ算出覆盖的梯形范围
        for(int i=0;i<m;++i){
            const pnt& p1=flights[i].first, p2=flights[i].second;
            auto z1=fheights[i].first, z2=fheights[i].second;
            pnt ortho=(p2 - p1).perpendicular();
            ortho= ortho / ortho.len();
            vector<pnt> trapezoid(4);
            trapezoid[0]= p1 + ortho * z1 * tan(theta);
            trapezoid[1]= p2 + ortho * z2 * tan(theta),
            trapezoid[2]= p2 - ortho * z2 * tan(theta);
            trapezoid[3]= p1 - ortho * z1 * tan(theta);
            for(int is=0;is<n;++is){
                //判断梯形是否包含一个完整的island
                bool co=true;
                for(const auto& p:islands[is])
                    if (!pntInTrapz(trapezoid, p)) {
                        co=false;
                        break;
                    }
                if(!covered[is] && co)
                    covered[is]=true;
            }
        }
        if(covered==vector<bool>(n,true))
            hi=theta;
        else
            lo=theta;
    }
    if(hi==pi/2)
        cout<<"impossible"<<endl;
    else
        cout<<setprecision(9)<<(lo+hi)/2*90/(pi/2)<<endl;
}