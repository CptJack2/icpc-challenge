#include <bits/stdc++.h>

using namespace std;

struct record
{
    int total = 0;
    int child = -1;
};

typedef array<record, 4> records;

static void push(records &r, const record &n)
{
    for (int i = 0; i < int(r.size()); i++)
    {
        if (n.total >= r[i].total)
        {
            for (int j = r.size() - 1; j > i; j--)
                r[j] = r[j - 1];
            r[i] = n;
            return;
        }
    }
}

struct node
{
    int max_path = 0;     // longest path in the subtree
    int max_path_up = 0;  // two paths, one of which ends at the root
    int height = 0;       // max path length in subtree starting at this node
    int full = 0;         // two paths in subtree
    vector<pair<int, int>> edges;
};

static vector<node> nodes;

static void recurse(int cur, int parent)
{
    node &n = nodes[cur];
    records path, path_up, height;
    for (const auto &edge : n.edges)
    {
        int y = edge.first;
        int d = edge.second;
        if (y == parent)
            continue;
        recurse(y, cur);
        int h = nodes[y].height + d;
        push(height, record{h, y});
        push(path, record{nodes[y].max_path, y});
        push(path_up, record{nodes[y].max_path_up + d, y});
        n.full = max(n.full, nodes[y].full);
    }
    n.height = height[0].total;
    n.max_path = max(path[0].total, height[0].total + height[1].total);
    /* Two paths, one ending at the root. Either the second path is in the same
     * subtree that the first path descends to, or they're unrelated.
     */
    n.max_path_up = path_up[0].total;

	auto cross=[&](const records& ra,const records& rb){
		int ret=0;

		int poss1=ra[0].total;
		int i=0;
		while(ra[0].child!=-1 && rb[i].child==ra[0].child)++i;
		poss1+=rb[i].total;
		ret=max(ret,poss1);

		int poss2=rb[0].total;
		i=0;
		while(rb[0].child!=-1 && ra[i].child==rb[0].child)++i;
		poss2+=ra[i].total;
		ret=max(ret,poss2);

		return ret;
	};

	n.max_path_up=max(n.max_path_up,cross(height,path));

    n.max_path_up = max(n.max_path_up, height[0].total + height[1].total + height[2].total);

    /* A pair of maximal paths in this subtree consists of either:
     * - Two paths in the same subtree (handled in the loop above).
     * - One path in each of two separate subtrees.
     * - Both paths pass though the root
     * - One passing through the root and visiting the subtree of the other.
     * - One passing through the root, but not visiting the subtree of the other.
     */
    n.full = max(n.full, n.max_path_up);
    n.full = max(n.full, path[0].total + path[1].total);
    n.full = max(n.full, height[0].total + height[1].total + height[2].total + height[3].total);

	n.full=max(n.full,cross(height,path_up));
    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++)
            for (int k = 0; k < 3; k++)
                if (path[k].child == -1
                    || (path[k].child != height[i].child && path[k].child != height[j].child))
                {
                    int score = height[i].total + height[j].total + path[k].total;
                    n.full = max(n.full, score);
                }
}

int main()
{
    int N;
    cin >> N;
    nodes.resize(N);
    int tdist = 0;
    for (int i = 0; i < N - 1; i++)
    {
        int u, v, d;
        cin >> u >> v >> d;
        u--;
        v--;
        nodes[u].edges.emplace_back(v, d);
        nodes[v].edges.emplace_back(u, d);
        tdist += d;
    }
    recurse(0, -1);
    int ans = 2 * tdist - nodes[0].full;
    cout << ans <<'\n';

    return 0;
}
