#include <bits/stdc++.h>

using namespace std;

/**
 * ICPC World Finals 2020
 * Sample Solution: Speed Limit Signs
 * <p>
 * Combine results of subtrees into a tree result.
 * The "result" is described by the {@link SubResult} class - see below.
 * <p>
 * This probably should be "the slowest solution we would like to accept".
 * Its time complexity is O(n^2) but it uses recursive DFS in Java
 * and plenty of objects allocated on the heap.
 *
 * @author Martin Kacer
 */

struct Edge;

struct Node {
	const int idx;
	vector<Edge> edges;

	Node(int idx) : idx(idx) {}
};

struct Edge {
	Node &node;
	const int speed;
	Edge(Node &node, int speed) : node(node), speed(speed) {}
};

static vector<Node> graph;
static int n, csign;

/**
 * Description of a subtree result, which is basically a function
 * mapping a speed limit of an incoming ("up") edge to the best possible solution
 * of the whole subtree.
 * Because of the nature of the problem, the result function is piecewise linear,
 * consisting of left-closed right-open intervals.
 */
struct SubResult {
	Edge *inc;
	const int deg;
	const long unbal;
	vector<int> si, mi;
	vector<long> ci;
	int idx = 0;

	/**
	 * @param deg real degree of the current node, including the parent edge, if any.
	 * @param edge edge to the parent node. Null if this is the tree root.
	 * @param sum the sum of best solutions for all child nodes together.
	 */
	SubResult(int deg, Edge *edge, long sum) : inc(edge), deg(deg), unbal(sum + deg * csign) {}

	/** Basic cost for a given speed.
	 * @param speed speed of an "incoming edge" (the one from the node's parent).
	 *   Must not be less than the current linear segment - which means the speed of the parent
	 *   link at the beginning, and then the speed argument passed to this method for the last time.
	 * @return the best cost achievable with the given incoming edge speed, counting the whole subtree.
	 *   The cost may represent both balanced or unbalanced node, whichever is cheaper.
	 *   If the node is unbalanced, the cost will include the installation of all speed-limit
	 *   signs, including the bottom end of the incoming (parent) edge (but only if there exists any!).
	 *   However, note that the cost does NOT include any cost of upgrading the incoming edge.
	 *   Even when the result represents a balanced node, the incoming edge is upgrade is NOT counted.
	 */
	long costFor(int speed) {
		while (idx < (int) si.size() && speed >= si[idx]) ++idx;
		if (idx == 0) return unbal; // incoming edge should have lower speed than our minimum
		return ci[idx-1] + (speed - si[idx-1]) * mi[idx-1];
	}

	/** Slope of the current linear segment.
	 * @return the "marginal cost" of increasing the incoming edge speed by 1 more km/h.
	 *   The value corresponds to the last value returned by {@link #costFor}.
	 *   As for {@link #costFor}, the marginal cost does NOT include the incoming edge upgrade.
	 */
	int marginalCost() {
		return (idx == 0) ? 0 : mi[idx-1];
	}

	/** The left end of the next interval.
	 * @return the next "interesting" point of the result function greater than the last speed
	 *   passed to {@link #costFor}. MAX_VALUE if we are already in the last segment.
	 */
	int nextSpeed() {
		return (idx < (int) si.size()) ? si[idx] : INT_MAX;
	}

	/** The best cost achievable for the whole subtree, without any limitation on the incoming edge.
	 * Unlike {@link #costFor}, this result will include the cost of upgrading the incoming edge,
	 * if it is necessary to do so.
	 */
	long bestCost() {
		long best = unbal;
		for (size_t i = 0; i < ci.size(); ++i)
			best = min(best, ci[i] + (inc == nullptr ? 0 : si[i] - inc->speed));
		return best;
	}

	/** Add a new linear segment to an already-existing function.
	 * This may only be invoked before the other (reading) methods. */
	void addSegment(int speed, long cost, int marg) {
		if (si.size() > 0 && si[si.size()-1] >= speed) {
			si.pop_back(); ci.pop_back(); mi.pop_back();
		}
		if (marg == 0) { doAdd(speed, cost, 0); return; }
		long sd = (unbal - cost + marg-1) / marg;
		if (sd <= 0) doAdd(speed, unbal, 0);
		else {
			doAdd(speed, cost, marg);
			doAdd(speed + (int) sd, unbal, 0);
		}
	}
	void doAdd(int speed, long cost, int marg) {
		if (ci.empty() || ci.back() != cost || mi.back() != marg) {
			si.push_back(speed); ci.push_back(cost); mi.push_back(marg);
		}
	}
};

/** The recursive step -- combine results of subtrees. */
SubResult solve(Node &cur, Edge *inc, Node *parent) {
	vector<SubResult> subs;
	subs.reserve(cur.edges.size());
	int max = 0; long sum = 0;
	for (Edge &e : cur.edges) {
		max = std::max(max, e.speed);
		if (&e.node == parent) continue;
		SubResult r = solve(e.node, &e, &cur);
		subs.push_back(r);
		sum += r.bestCost();
	}
	SubResult myres(cur.edges.size(), inc, sum);
	// gradually increase the speed and find next segments of the resulting function
	for (int
	s = max; s < INT_MAX; ) {
		long sc = 0; int sm = 0;
		for (SubResult &sr : subs) {
			sc += sr.costFor(s) + (s - sr.inc->speed);
			sm += sr.marginalCost() + 1; // plus 1 for the respective edge itself
		}
		myres.addSegment(s, sc, sm);
		s = INT_MAX;
		for (auto &sr: subs)
			s = min(s, sr.nextSpeed());
	}
	return myres;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cin >> n >> csign;
	for (int i = 0; i < n; ++i) graph.emplace_back(i);
	for (int i = 1; i < n; ++i) {
		int u, v, s;
		cin >> u >> v >> s;
		u--;
		v--;
		graph[u].edges.emplace_back(graph[v], s);
		graph[v].edges.emplace_back(graph[u], s);
	}
	cout << solve(graph[0], nullptr, nullptr).bestCost() << '\n';
	return 0;
}
