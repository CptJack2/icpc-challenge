#include <bits/stdc++.h>

using namespace std;

struct edge{
	int dest;
	int len;
	edge(int d,int l):dest(d),len(l){}
};

struct node{
    vector<edge> edges;
};

static vector<int> recurse(vector<node> &nodes, int cur, int parent, int E) {
    node &n = nodes[cur];
    vector<int> dp(E + 1, 0);//dp[0]永远等于0.dp[1]为cur到任意叶子的最长高度.dp[2]为子树中最长路径
    for (const auto &ch : n.edges) {
		if (ch.dest == parent) continue;
		auto sub = recurse(nodes, ch.dest, cur, E);
		for (int i = E-1; i >= 0; i--)
			for (int j = E - i; j >= 1; j--) {
				int score = dp[i] + sub[j] + ((j & 1) ? ch.len : 0);
				dp[i + j] = max(dp[i + j], score);
			}
	}
    return dp;
}

int main() {
    int N;
    cin >> N;
    vector<node> nodes(N+1);
    int K = 2;//two longest sub path without common edge, modify it to get k longest sub path
    int tdist = 0;
    for (int i = 0; i < N - 1; i++) {
        int u, v, d;
        cin >> u >> v >> d;
        nodes[u].edges.emplace_back(v, d);
        nodes[v].edges.emplace_back(u, d);
        tdist += d;
    }
    auto dp = recurse(nodes, 1, -1, 2 * K);
    int ans = 2 * tdist - dp.back();
    cout << ans <<'\n';

    return 0;
}
