#include "bits/stdc++.h"
using namespace std;

//void Init() {
//    srand(time(0));
//    ios::sync_with_stdio(false);
//    ::cin.tie(NULL);
//}

int64_t cmpx = 1, cmpy = 0;
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
    Point mndiff = mxtot + mntot, mxdiff = mndiff;
    for (int i = 1; i < ch[x].size(); i++) {
        auto[mn, mx] = doit(ch[x][i]);
        mntot += mn;
        mxtot += mx;
        mndiff = min(mndiff, mx + mn);
        mxdiff = max(mxdiff, mx + mn);
    }
    return {-mxtot + mndiff, -mntot + mxdiff};
}

pair<Point, Point> tryAngle(Point dir) {
    cmpx = dir.x;
    cmpy = dir.y;
    function<pair<Point, Point>(int)> doit = [&](int x) -> pair<Point, Point> {
        if (ch[x].size() == 0) return {p[x], p[x]};
        auto[mntot, mxtot] = doit(ch[x][0]);
        Point mndiff = mxtot + mntot, mxdiff = mndiff;
        for (int i = 1; i < ch[x].size(); i++) {
            auto[mn, mx] = doit(ch[x][i]);
            mntot += mn;
            mxtot += mx;
            mndiff = min(mndiff, mx + mn);
            mxdiff = max(mxdiff, mx + mn);
        }
        return {-mxtot + mndiff, -mntot + mxdiff};
    };
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

stringstream cin(R"(
4
3 2 3 4
0 10 1
0 3 6
0 2 7
)");

int main() {
//    Init();

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