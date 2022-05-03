#include "bits/stdc++.h"
using namespace std;

int N, C;
vector<vector<pair<int, int>>> graph;
vector<int> allUniqueSpeed;

//return the best cost for subtree with x as root, and best cost under corresponding speed in allUniqueSpeed
//note that this function doesn't consider the influence of the street entering x
pair<int, vector<int>> dfs(int x, int prev) {
	int maxSpeed = 0, signCost = 0, parentSpeed = 0;
	//find maximum speed of this intersection, and the speed of the street entering the intersection
	for (auto [y, speed] : graph[x]) {
		maxSpeed = max(maxSpeed, speed);
		if (y == prev) parentSpeed = speed;
	}
	//cost to setup speed sign on this intersection
	if (graph[x].size() > 1)
		signCost += C * graph[x].size();
	//compute the cost of changing intersection speed to the speed in allUniqueSpeed
	vector<int> costAtSpeed;
	for (auto [y, _ ] : graph[x])
		if (y != prev) {
			//compute the result of all the subtree first
			auto [bestCostOfSubTree, costAtSpeedOfSubTree] = dfs(y, x);
			//cost of putting sign should include the best cost of all sub trees. note that
			signCost += bestCostOfSubTree;
			//update the cost of increasing the intersection speed to allUniqueSpeed[i]
			if (costAtSpeed.empty()) costAtSpeed.resize(allUniqueSpeed.size());
			for (int i = 0; i < allUniqueSpeed.size(); i++)
                //if we tried to increase the speed to allUniqueSpeed[i], we should use the cost at allUniqueSpeed[i] for every subtree
			    costAtSpeed[i] += costAtSpeedOfSubTree[i];
		}
	if (costAtSpeed.empty()) costAtSpeed.resize(allUniqueSpeed.size());
	//compare cost at each speed and cost of putting sign, to find the best cost
	int bestCost = signCost;
	for (int i = 0; i < allUniqueSpeed.size(); i++) {
		if (allUniqueSpeed[i] >= maxSpeed) {
			//cost of increasing the whole subtree‘s speed rooted from x to allUniqueSpeed[i] should include the cost of increasing speed of entering x, now it's finally complete
            costAtSpeed[i] += (prev ? allUniqueSpeed[i] - parentSpeed : 0);
            //if this is a subtree, the cost of the entering tree is needed to be considered, cause we can find a best speed for the subtree, and put sign on its parent
			bestCost = min(bestCost, costAtSpeed[i]);
			//if putting sign is cheaper at this speed, make it the better choice for cost at this speed. the street entering x should be considered too
			costAtSpeed[i] = min(costAtSpeed[i], signCost + (prev ? allUniqueSpeed[i] - parentSpeed : 0) );
		} else
			//decreasing speed is not allowed, so putting sign is the only choice
			costAtSpeed[i] = signCost + (prev ? allUniqueSpeed[i] - parentSpeed : 0);
	}
	return {bestCost, costAtSpeed};
}

int main() {
	int u,v,s;
	cin>>N>>C;
	graph.resize(N + 1);
	for (int i=1; i <N; i++) {
		cin>>u>>v>>s;
		graph[u].push_back({v, s});
		graph[v].push_back({u, s});
		allUniqueSpeed.push_back(s);
	}
	//all unique speed in the graph, sorted ascending
	sort(allUniqueSpeed.begin(), allUniqueSpeed.end());
	allUniqueSpeed.erase(unique(allUniqueSpeed.begin(), allUniqueSpeed.end()), allUniqueSpeed.end());
	auto[bestCost, _] = dfs(1, 0);
	cout << bestCost;
}
