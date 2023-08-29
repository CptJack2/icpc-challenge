#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
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
long double RayIntersect(const Point& a, const Point& b, const Point& c, const Point& d, int* sides = NULL) {
    long double cp1 = CrossProd(c-a, b-a), cp2 = CrossProd(d-a, b-a);
    long double dp1 = DotProd(c-a, b-a), dp2 = DotProd(d-a, b-a);
    if (sides) *sides = (cp1 < -EPS || cp2 < -EPS) + 2 * (cp1 > EPS || cp2 > EPS);//sides 0,1,2,3,如果两个线段相交,一定是3  1,2是ab直线穿过c或者d的情况
    if (cp1 < -EPS && cp2 < -EPS || cp1 > EPS && cp2 > EPS) return -1.0;//叉积结果同号，线段不相交
    return (abs(cp1) < EPS && abs(cp2) < EPS) ? min(dp1, dp2) : (dp1*cp2-dp2*cp1)/(cp2-cp1);//a到交点的距离乘以ab长度
}

bool PointOnLine(const Point& a, const Point& b, const Point& p) {
    long double ln = (b-a).Len(), cp = CrossProd(b-a, p-a), dp = DotProd(b-a, p-a);
    return abs(cp/ln) < EPS && dp/ln > -EPS && dp/ln < ln+EPS;//cp=0 0<dp<ln^2
}

int N;
vector<Point> p;

int main() {
    cin>>N;
    p.resize(N + 2);
    for (int i = 0; i < N + 2; i++) cin >> p[i].x >> p[i].y;

    //找指向每个顶点方向距离最近的intersecting边
    //找出每个可能的终点,推入p
    for (int i = 0; i < N + 1; i++) {
        Point a = p[N + 1], b = p[i];//a是sculpture的坐标
        if ((b - a).Len() < EPS) {
            p.push_back(b);
            continue;
        }
        b = (b - a) / (b - a).Len() + a;//从sculpture出发指向polygon端点的单位向量
        vector<pair<long double, int>> inter;
        for (int j = 0; j < N; j++) {
            int sides = 0;
            long double rd = RayIntersect(a, b, p[j], p[(j + 1) % N], &sides);//由于ab是单位向量,rd就是a到交点的距离
            if (rd < 0) continue;//两线段不相交
            inter.push_back({rd, sides});
        }//👏
        sort(inter.begin(), inter.end());
        long double maxd = 0.0;
        //防止多个顶点刚好卡住视野的情况
        for (int j = 0, sides = 0; sides != 3; j++) {
            maxd = inter[j].first;
            sides |= inter[j].second;
        }
        p.push_back((b - a) * maxd + a);//sculpture到交点的距离,
        for (int j = 0; j <= N; j++) {
            //与ab垂直的两个方向的向量
            long double rd = RayIntersect(a, b, p[j], p[j] + Point(b.y - a.y, a.x - b.x) * 1.1e3);
            if (rd < 0) rd = RayIntersect(a, b, p[j], p[j] + Point(a.y - b.y, b.x - a.x) * 1.1e3);
            if (rd > EPS && rd < maxd - EPS) p.push_back((b - a) * rd + a);
        }
    }

    //dijkstra算法求最短长度
    vector<long double> dist(p.size(), 1e10);
    priority_queue<pair<long double, int>> q;//priority_queue是大根堆,为了让最小的排前面,推进去的是乘-1的
    q.push({0.0, N});
    for (;;) {
        int i = q.top().second;
        if (i > N) break;//只要到达了任意的非polygon顶点肯定就是终点
        long double d = -q.top().first;
        q.pop();
        if (d >= dist[i]) continue;
        dist[i] = d;
        for (int j = 0; j < p.size(); j++) {
            Point a = p[i], b = p[j];
            long double ln = (b - a).Len();
            int ni = 0;
            if (ln < EPS) goto pass;
            //如果j在poly顶点i关联的两条poly边上,可能是邻接顶点或者其它特殊点
            if (i < N && PointOnLine(p[i], p[(i + 1) % N], p[j])) goto pass;
            if (i < N && PointOnLine(p[i], p[(i + N - 1) % N], p[j])) goto pass;
            b = (b - a) / ln + a;
            for (int k = 0; k < N; k++) {
                //从i到j中间有poly边遮挡
                long double rd = RayIntersect(a, b, p[k], p[(k + 1) % N]);
                if (rd > EPS && rd < ln - EPS) goto fail;
            }
            //判断一下ij的中点是不是在poly内部。通过ij中点划斜线看穿过poly为奇数偶数次
            a = (p[i] + p[j]) / 2;
            b = a + Point(cos(10), sin(10));
            for (int k = 0; k < N; k++) {
                long double rd = RayIntersect(a, b, p[k], p[(k + 1) % N]);
                ni += (rd > EPS);
            }
            if (ni % 2 == 0) goto fail;
            pass:
            q.push({-d - ln, j});
            fail:;
        }
    }
    printf("%.12Lf\n", -q.top().first);
}