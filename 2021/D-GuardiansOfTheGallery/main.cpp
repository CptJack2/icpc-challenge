#include <bits/stdc++.h>
//#include <ranges>

using namespace std;

const double EPS= 1e-9;

struct Point {
    long double x, y;
    Point(long double x = 0.0, long double y = 0.0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
    Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
    Point operator*(long double c) const { return {x*c, y*c}; }
    Point operator/(long double c) const { return {x/c, y/c}; }
    long double Len() const { return hypot(x, y); }
};

inline long double CrossProd(const Point& a, const Point& b) {
    return a.x*b.y - a.y*b.x;
}

inline long double DotProd(const Point& a, const Point& b) {
    return a.x*b.x + a.y*b.y;
}

//如果两个向量的叉积结果符号相反（一个为正，一个为负），则说明线段AC和线段AD在线段AB的两侧，即线段AC和线段AD相交。
double RayIntersect(const Point& a, const Point& b, const Point& c, const Point& d, int* sides = NULL) {
    double cp1 = CrossProd(c-a, b-a), cp2 = CrossProd(d-a, b-a);
    double dp1 = DotProd(c-a, b-a), dp2 = DotProd(d-a, b-a);
    if (sides) *sides = (cp1 < -EPS || cp2 < -EPS) + 2 * (cp1 > EPS || cp2 > EPS);//sides 0,1,2,3,如果两个线段相交,一定是3  1,2是ab直线穿过c或者d的情况
    if (cp1 < -EPS && cp2 < -EPS || cp1 > EPS && cp2 > EPS) return -1.0;//叉积结果同号，线段不相交
    return (abs(cp1) < EPS && abs(cp2) < EPS) ? 0 : (dp1*cp2-dp2*cp1)/(cp2-cp1);//a到交点的距离乘以ab长度
}

bool PointOnLine(const Point& a, const Point& b, const Point& p) {
    double ln = (b-a).Len(), cp = CrossProd(b-a, p-a), dp = DotProd(b-a, p-a);
    return abs(cp/ln) < EPS && dp/ln > -EPS && dp/ln < ln+EPS;//cp=0 && 0<dp<ln^2
}

int main(){
    int n;
    cin>>n;
    vector<Point> polygon(n);
    Point s,g;
    for (int i = 0; i < n; ++i) {
        cin>>polygon[i].x>>polygon[i].y;
    }
    cin>>g.x>>g.y;
    cin>>s.x>>s.y;
    vector<Point> endpoints;
    for(auto p:polygon){
        vector<tuple<double,int>> intersections;//交点距离，遮挡面
        p=(p-s)/(p-s).Len()+s;
        //找出sculpture向各个顶点出发, 不被遮挡能走的最远距离maxd，即视野不被遮挡的区域的终点(在多边形的边上)
        for(int i=0;i<n;++i){
            auto& a=polygon[i], &b=polygon[i!=n?i+1:0];
            int side;
            auto d= RayIntersect(s,p,a,b,&side);
            if(d>EPS)
                intersections.emplace_back(d,side);
        }
        ranges::sort(intersections);
        double maxd=0;
        int sides=0;
        for(auto [d,s]:intersections){
            maxd=d;
            sides|=s;
            if(sides==3)break;
        }
        endpoints.push_back(s+(p-s)*maxd);
        //计算顶点p2，到sculpture发出的穿过顶点i的射线，的距离,以及交点
        for(auto p2:polygon){
            if((p-p2).Len()<EPS)continue;
            auto ortho=Point(s.y-p.y,p.x-s.x)*1e5;
            auto d= RayIntersect(s,p,p2,p2+ortho);
            if(d<-EPS)
                d= RayIntersect(s,p,p2,p2-ortho);
            if(d>EPS && d<maxd-EPS)
                endpoints.push_back(s+(p-s)*d);
        }
    }
    //dijkstra算法求最短长度
    vector<Point> points(polygon.size()+2+endpoints.size());
    int i=0;
    points[i++]=g;
    for(auto p:polygon)points[i++]=p;
    points[i++]=s;
    for(auto p:endpoints)points[i++]=p;
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> que;
    vector<double> dist(points.size(),1e20);//从g到每个节点的最短路径长度
    que.emplace(0.0,0);//从0号guard的起始位置出发
    for(i=que.top().second;i<n;i=que.top().second){
        auto d=que.top().first;
        que.pop();
        if(d>=dist[i])continue;
        dist[i]=d;
        for(int j=0;j<points.size();++j){
            auto p1=points[i], p2=points[j];
            auto ln=(p1-p2).Len();
            if(ln<EPS ||
               //如果j在poly顶点i关联的两条poly边上,可以直接沿着poly边走到j,不需要后面的包含性判断
               i>=1&&i<=n && PointOnLine(polygon[i-1], polygon[(i) % n], p2) ||
               i>=1&&i<=n && PointOnLine(polygon[i-1], polygon[(i + n - 2) % n], p2)) {
                que.emplace(d+ln,j);
                break;
            }
            //判断p1p2是否被poly完全包含
            p2=p1+(p2-p1)/(p2-p1).Len();
            for(int k=0;k<polygon.size();++k){
                auto rd= RayIntersect(p1,p2,polygon[k],polygon[(k+1)%n]);
                if(rd>EPS && rd<ln-EPS)break;
            }
            //判断一下p1p2上任意一点是不是在poly内部，来保证p1p2全段都在poly内部。通过p1p2中点画一任意射线, 看穿过poly为奇数还是偶数次
            p1 = p1*2/3 + p2/ 3;
            p2 = p1 + Point(cos(10), sin(10));
            int cnt=0;
            for(int k=0;k<n;++k){
                auto rd= RayIntersect(p1,p2,polygon[k],polygon[(k+1)%n]);
                cnt+=(rd>EPS);
            }
            if(cnt%2==0)
                que.emplace(d+ln,j);
        }
    }
    printf("%.12f\n", que.top().first);
}