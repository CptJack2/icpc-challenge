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

static vector<int> recurse(vector<node> &nodes, int cur, int parent, int E)
{
    node &n = nodes[cur];
    vector<int> dp(E + 1, 0);
    for (const auto &e : n.edges)
        if (e.dest != parent)
        {
            auto sub = recurse(nodes, e.dest, cur, E);
            for (int i = E; i >= 0; i--)
                for (int j = E - i; j >= 0; j--)
                {
                    int score = dp[i] + sub[j] + ((j & 1) ? e.len : 0);//if(j==1,3) score+=e.second
                    dp[i + j] = max(dp[i + j], score);
                }
        }
    return dp;
}

int main()
{
    int N;
    cin >> N;
    vector<node> nodes(N+1);
    int K = 2;
    int tdist = 0;
    for (int i = 0; i < N - 1; i++)
    {
        int u, v, d;
        cin >> u >> v >> d;
        nodes[u].edges.emplace_back(v, d);
        nodes[v].edges.emplace_back(u, d);
        tdist += d;
    }
    auto dp = recurse(nodes, 4, -1, 2 * K);
    int ans = 2 * tdist - dp.back();
    cout << ans <<'\n';

    return 0;
}
