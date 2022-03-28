#include "bits/stdc++.h"
using namespace std;

int N, C;
vector<vector<pair<int, int>>> graph;
vector<int> allUniqueSpeed;

//return the best cost for subtree, and even cost under corresponding speed in allSpeed
pair<int, vector<int>> dfs(int x, int prev) {
	int mxs = 0, signcost = 0, ps = 0;
	for (auto [y, s] : graph[x]) {
		mxs = max(mxs, s);
		if (y == prev) ps = s;
	};
	if (graph[x].size() > 1) signcost += C * graph[x].size();
	vector<int> v;
	for (auto [y, s] : graph[x]) if (y != prev) {
			auto [c2, v2] = dfs(y, x);
			if (!v.size()) v.resize(allUniqueSpeed.size());
			for (int i = 0; i < allUniqueSpeed.size(); i++)
				v[i] += v2[i] + allUniqueSpeed[i] - s;
			signcost += c2;
		};
	if (!v.size()) v.resize(allUniqueSpeed.size());
	int evencost = signcost;
	for (int i = 0; i < allUniqueSpeed.size(); i++) {
		if (allUniqueSpeed[i] >= mxs) {
			evencost = min(evencost, v[i] + (prev ? allUniqueSpeed[i] - ps : 0));
			v[i] = min(v[i], signcost);
		} else {
			v[i] = signcost;
		}
	}
	return {evencost, v};
}

int main() {
	int u, v, s;
	cin >> N >> C;
	graph.resize(N + 1);
	for (int i = 1; i < N; i++) {
		cin >> u >> v >> s;
		graph[u].push_back({v, s});
		graph[v].push_back({u, s});
		allUniqueSpeed.push_back(s);
	}
	sort(allUniqueSpeed.begin(), allUniqueSpeed.end());
	allUniqueSpeed.erase(unique(allUniqueSpeed.begin(), allUniqueSpeed.end()), allUniqueSpeed.end());
	auto[c, v] = dfs(1, 0);
	cout << c << endl;
}
