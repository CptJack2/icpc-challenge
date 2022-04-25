#include "bits/stdc++.h"
using namespace std;

#define EPS 1e-18

struct Distribution {
  vector<long double> v;
  int base = 0, ignoredLB = 0, ignoredUB;

  Distribution() : v{1.0},ignoredUB(1) {}

  Distribution(const Distribution& d) {
    base = d.base + d.ignoredLB;
    v.insert(v.begin(), d.v.begin()+d.ignoredLB, d.v.begin()+d.ignoredUB);
  	ignoredUB=v.size();
  }

  long double operator[](int i) const {//在管理的方格集合内有i个mine的概率
    if (i < base + ignoredLB) return 0.0;
    if (i >= base + ignoredUB) return 0.0;
    return v[i-base];
  }

  void Add(long double p) {
    v.push_back(0);
    if(ignoredUB==v.size()-1)
    	ignoredUB=v.size();
    for (int i = ignoredUB-1; i >= ignoredLB; i--)
    	v[i] = i>=1 ? (1 - p) * v[i] + p * v[i - 1] : (1-p)*v[i];
//    v[ignoredLB] *= (1 - p);
    //忽略掉尾部和头部的概率
    while (ignoredLB<ignoredUB && v[ignoredLB] <= EPS) ignoredLB++;
    while (ignoredUB>ignoredLB && v[ignoredUB-1] <= EPS) --ignoredUB;
  }
};

int X, Y, T, Q;
vector<long double> XP, YP;
vector<vector<pair<int, int>>> queries;

map<pair<int, int>, vector<pair<int, int>>> node;//queries[s,e)里的查询涉及的所有方格坐标
const vector<pair<int, int>>& domerge(int s, int e) {
  auto& v = node[{s, e}];
  if (s+1 == e) {
    v = queries[s];
  } else {
    auto const& a = domerge(s, (s+e)/2);
    auto const& b = domerge((s+e)/2, e);
    v = a;
    v.insert(v.end(), b.begin(), b.end());
  }
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}
long double tot = 0;
void doit(int s, int e, const Distribution& dn) {
  if (s+1 == e) {
    if (s == Q) return;
    Distribution dy;
    for (auto [x, y] : queries[s]) dy.Add(XP[x] + YP[y]);
    if(tot==0)
    	for (int i = 0; i <= queries[s].size(); i++) tot += dy[i] * dn[T - i];//有i个在前面的,T-i个在后面的
    for (int i = 0; i <= queries[s].size(); i++) {
//      printf("%.9lf ", dy[i] * dn[T-i] / tot);
		cout<<setprecision(9)<<dy[i] * dn[T-i] / tot<<" ";
    }
//    printf("\n");
	  cout<<endl;
    return;
  }
  Distribution dna(dn), dnb(dn);
  auto const& a = node[{s, (s+e)/2}];
  auto const& b = node[{(s+e)/2, e}];
  for (int ai = 0, bi = 0; ai < a.size() || bi < b.size(); ) {
    //将a,b的差异部分加入到对方的distribution内
  	if (bi == b.size() || (ai < a.size() && a[ai] < b[bi])) {
      dnb.Add(XP[a[ai].first] + YP[a[ai].second]);
      ai++;
    } else if (ai == a.size() || b[bi] < a[ai]) {
      dna.Add(XP[b[bi].first] + YP[b[bi].second]);
      bi++;
    } else {
      ai++;
      bi++;
    }
  }
  doit(s, (s+e)/2, dna);
  doit((s+e)/2, e, dnb);
}

int main() {
  while (cin >> X >> Y >> T >> Q) {
    XP.resize(X); YP.resize(Y);
    for (auto& p : XP) cin >> p;
    for (auto& p : YP) cin >> p;
    queries.clear();
    queries.resize(Q + 1);
    for (int i = 0; i < Q; i++) {
      int S;
      cin >> S;
      for (int j = 0; j < S; j++) {
        int x, y;
        cin >> x >> y;
        queries[i].emplace_back(x - 1, y - 1);
      }
    }
    //需要一个全集来处理差异
    for (int x = 0; x < X; x++) for (int y = 0; y < Y; y++) queries[Q].emplace_back(x, y);
    domerge(0, Q+1);
    doit(0, Q+1, Distribution());
  }
}
