#include "bits/stdc++.h"
using namespace std;

int64_t cmpx = 1, cmpy = 0;//一个随机向量

struct Point {
    int64_t x, y;
    Point operator-() const { return {-x, -y}; }
    Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
    Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
    Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
    bool operator<(const Point& p) const { return x*cmpx + y*cmpy < p.x*cmpx + p.y*cmpy; }
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    Point ortho() const { return {-y, x}; }
    int64_t lensqr() const { return x*x+y*y; }
};

vector<vector<int>> ch;
vector<Point> p;
int64_t ret = 0;

pair<Point, Point> doit(int x) {
    if (ch[x].size() == 0) return {p[x], p[x]};
    auto[mntot, mxtot] = doit(ch[x][0]);
    Point mndiff = mxtot+mntot, mxdiff = mndiff;
    for (int i = 1; i < ch[x].size(); i++) {
        auto[mn, mx] = doit(ch[x][i]);
        mntot += mn;
        mxtot += mx;
        mndiff = min(mndiff, mx + mn);//后面计算当前的min total，要将max total里对应节点的max去除，然后替换成min，所以diff是用min+max的方式计算
        mxdiff = max(mxdiff, mx + mn);//max total同理
    }
    return {-mxtot + mndiff, -mntot + mxdiff};
}

pair<Point, Point> tryAngle(Point dir) {
    cmpx = dir.x;
    cmpy = dir.y;
    auto[mn, mx] = doit(1);
    ret = max(ret, mx.lensqr());
    ret = max(ret, mn.lensqr());
    return {mn, mx};
}

void traceHull(Point a, Point b) {
    if (a == b) return;
    auto[_, c] = tryAngle((b - a).ortho());
    if (a < c) {
        traceHull(a, c);
        traceHull(c, b);
    }
}

int main() {
    int N, M;
    ::cin >> N;
    ch.resize(N + 1);
    p.resize(N + 1);
    for (int i = 1; i <= N; i++) {
        ::cin >> M;
        if (M == 0) {
            ::cin >> p[i].x >> p[i].y;
        } else {
            ch[i].resize(M);
            for (auto &x : ch[i]) ::cin >> x;
        }
    }

    auto[left, right] = tryAngle({1, 0});
    traceHull(left, right);
    traceHull(right, left);

    cout<<ret<<endl;
}