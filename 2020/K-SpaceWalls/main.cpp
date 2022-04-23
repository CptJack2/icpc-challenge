/*
 * 将所有点的坐标先x2,每个立方体有两个面,负轴方向的面+0,正轴方向的+1. 注意:0的x+面和1的x-面是同一面
 * 3个坐标轴分别对各个方块的坐标排序
 * 然后对每个robot，先求出它的起始点，找出它落在哪个方块的区域中
 * 然后从它的方向，找刚刚排好序的坐标，然后模拟在那个方向上前进，遇到转弯就记录转弯点，找回一圈之后就能得到整个轨迹的端点
 *
 * 因为最多有100个方块，所以一条轨迹最多有400个点。每两个轨迹之间进行相交判断.取出每条轨迹的每一段来判断是否相交,并求出交点, 整个复杂度是O(N^4)
 *
 * 获得轨迹的骄交点后,算出它们的相遇时间,每一对轨迹处理完后,给出最小相遇时间
 */

//#pragma GCC optimize("O3")
// TODO: check what conditions the final problem puts on robots starting on
// the same position.

#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using pnt2 = array<int, 2>;
using pnt3 = array<int, 3>;
using rect = array<pnt2, 2>;
using cuboid = array<pnt3, 2>;

static map<string, pnt3> dirmap{
{"x+", {{1, 0, 0}}},
{"x-", {{-1, 0, 0}}},
{"y+", {{0, 1, 0}}},
{"y-", {{0, -1, 0}}},
{"z+", {{0, 0, 1}}},
{"z-", {{0, 0, -1}}}
};

static const pnt2 delta[4] = {
{{1, 0}},
{{0, 1}},
{{-1, 0}},
{{0, -1}}
};

template<size_t N>
static int dot(const array<int, N> &a, const array<int, N> &b){
	int ans = 0;
	for (size_t i = 0; i < N; i++)
		ans += a[i] * b[i];
	return ans;
}

template<size_t N>
static array<int, N> operator+(const array<int, N> &a, const array<int, N> &b){
	array<int, N> out;
	for (size_t i = 0; i < N; i++)
		out[i] = a[i] + b[i];
	return out;
}

template<size_t N>
static array<int, N> operator*(const array<int, N> &a, int s){
	array<int, N> out;
	for (size_t i = 0; i < N; i++)
		out[i] = a[i] * s;
	return out;
}

template<size_t N>
static int distance(const array<int, N> &a, const array<int, N> &b){
	int ans = 0;
	for (size_t i = 0; i < N; i++)
		ans += abs(a[i] - b[i]);
	return ans;
}

// Unit vector from a -> b (must be axial)
template<size_t N>
static array<int, N> direction(const array<int, N> &a, const array<int, N> &b){
	array<int, N> out;
	for (size_t i = 0; i < N; i++)
		out[i] = (a[i] == b[i]) ? 0 : (a[i] < b[i]) ? 1 : -1;
	return out;
}

template<typename T>
static T &gget(vector<vector<T>> &grid, pnt2 pos){
	return grid[pos[0]][pos[1]];
}

template<typename T>
static T &gget(vector<vector<T>> &grid, int x, int y){
	return grid[x][y];
}

static pnt2 get_left(const pnt2 &p, int dir){
	int dx[4] = {0, -1, -1, 0};
	int dy[4] = {0, 0, -1, -1};
	return pnt2{{p[0] + dx[dir], p[1] + dy[dir]}};
}

static pnt2 get_right(const pnt2 &p, int dir){
	return get_left(p, (dir + 3) & 3);
}

static void add_seg(vector<pnt2> &segs, pnt2 p){
	int M = segs.size();
	if (M >= 2){
		const pnt2 &a = segs[M - 2];
		const pnt2 &b = segs[M - 1];
		//合并同一直线上的点
		if ((a[0] == b[0] && b[0] == p[0])
			|| (a[1] == b[1] && b[1] == p[1]))
			segs.pop_back();
	}
	segs.push_back(p);
}

// Undefined sign for negative inputs
template<typename T> static constexpr T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
template<typename T> static constexpr T wrap_pos(T a, T m) { return a < 0 ? a + m : a; }
template<typename T> static constexpr T sqr(T a) { return a * a; }
// m must be positive
template<typename T> static constexpr T mod(T a, T m) { return wrap_pos(a % m, m); }

template<typename T>
static constexpr T inverse2(T a, T m) { return a <= 1 ? a : mod((1 - inverse2(m % a, a) * m) / a, m); }

// a must be relatively prime to m, m > 0
template<typename T>
static constexpr T inverse(T a, T m) { return inverse2(mod(a, m), m); }

// Solve x = a (mod s) and x = b (mod t)
// Returns -1, -1 if no solutions exist
static pair<ll, ll> crt(ll a, ll s, ll b, ll t){
	assert(0 <= a);
	assert(0 <= b);
	a %= s;
	b %= t;
	ll g = gcd(s, t);
	if (g > 1){
		if (a % g != b % g)
			return {-1, -1};
		ll c, m;
		tie(c, m) = crt(a / g, s / g, b / g, t / g);
		assert(m > 0);
		return {c * g + a % g, m * g};
	}
	else
	{
		ll p = s * t;
		__int128 u = inverse(__int128(s), __int128(t)) * s % p;
		__int128 v = inverse(__int128(t), __int128(s)) * t % p;
		return make_pair((b * u + a * v) % p, p);
	}
}

int main(){
	int N, K;
	cin >> N >> K;
	vector<cuboid> cuboids(N);
	//读入空间站范围
	for (int i = 0; i < N; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 3; k++){
				int x;
				cin >> x;
				cuboids[i][j][k] = x * 2;//整个坐标体系x2
			}

	vector<vector<pnt3>> paths(K);
	vector<vector<ll>> times(K);
	for (int i = 0; i < K; i++){
		pnt3 robotInitPos;//机器人起始坐标
		string fs, ds;
		cin >> robotInitPos[0] >> robotInitPos[1] >> robotInitPos[2] >> fs >> ds;
		pnt3 initFace = dirmap[fs];
		pnt3 initDirection = dirmap[ds];
		for (int j = 0; j < 3; j++)
			robotInitPos[j] = 2 * robotInitPos[j] + 1 + initFace[j];//因为机器人初始在一个面的中心,所以这里+1
		//robot只会在d和f组成的平面内运动,有一条坐标轴可以忽略
		int axisIgnored = 0;
		while (initFace[axisIgnored] || initDirection[axisIgnored])
			axisIgnored++;

		vector<rect> rects;
		map<int, int> cordMaps[2];//map cord to index,将三维的坐标降到二维,用d和f做基
		//df坐标下的起始位置
		pnt2 initPos{{dot(robotInitPos, initDirection), dot(robotInitPos, initFace)}};
		for (int j = 0; j < 2; j++){
			cordMaps[j][initPos[j]] = -1;
			cordMaps[j][INT_MIN] = -1;
			cordMaps[j][INT_MAX] = -1;
		}
		//将各个方块降维成矩形
		for (int j = 0; j < N; j++){
			//第三轴不在范围内的排除
			if (cuboids[j][0][axisIgnored] > robotInitPos[axisIgnored] || cuboids[j][1][axisIgnored] < robotInitPos[axisIgnored])
				continue;
			rect r;
			for (int k = 0; k < 2; k++){
				//投影到d和f为基的坐标
				r[k][0] = dot(cuboids[j][k], initDirection);
				r[k][1] = dot(cuboids[j][k], initFace);
				cordMaps[0][r[k][0]] = -1;
				cordMaps[1][r[k][1]] = -1;
			}
			//调整成左下角右上角的坐标
			for (int k = 0; k < 2; k++)
				if (r[0][k] > r[1][k])
					swap(r[0][k], r[1][k]);
			rects.push_back(r);
		}
		//对坐标排序
		vector<int> coords[2];
		int X[2];
		for (int j = 0; j < 2; j++){
			X[j] = cordMaps[j].size();
			coords[j].resize(X[j]);
			int id = 0;
			for (auto &e : cordMaps[j]){
				coords[j][id] = e.first;
				e.second = id++;
			}
			initPos[j] = cordMaps[j][initPos[j]];
		}
		//cordMap所有顶点放到一张grid上
		vector<vector<int>> grid(X[0], vector<int>(X[1]));
		//下面的操作会将rect的区域,除了上边界和右边界是0,其他区域都是1
		for (auto &r : rects){
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					r[j][k] = cordMaps[k][r[j][k]];
			gget(grid, r[0])++;
			gget(grid, r[1])++;
			gget(grid, r[1][0], r[0][1])--;
			gget(grid, r[0][0], r[1][1])--;
		}
		//用泛洪一次过将矩形区域染色
		for (int y = 0; y < X[1]; y++)
			for (int x = 0; x < X[0]; x++){
				int v = gget(grid, x, y);
				if (y > 0)
					v += gget(grid, x, y - 1);
				if (x > 0)
					v += gget(grid, x - 1, y);
				if (x > 0 && y > 0)
					v -= gget(grid, x - 1, y - 1);
				gget(grid, x, y) = v;
			}
		//断言起始点在一条矩形的上边界. 起始点位置为0,其下方点不为0
		assert(gget(grid, initPos[0], initPos[1] - 1) > 0);
		assert(gget(grid, initPos) == 0);
		pnt2 p = initPos;
		int dir = 0;//dir 0,1,2,3 对应 d,f,-d,-f
		vector<pnt2> segs{initPos};
		//找出grid坐标下的轨迹,存入segs
		do
		{
			p = p + delta[dir];
			add_seg(segs, p);
			//df坐标,d是东,-f是南,-d西,f北. 右就是顺着当前矩形走的方向,左是遇上障碍物
			pnt2 left = get_left(p, dir);
			pnt2 right = get_right(p, dir);
			//看看左右两边是否能够走过去
			if (gget(grid, left))
				dir = (dir + 1) & 3;
			else if (!gget(grid, right))
				dir = (dir + 3) & 3;
			//不然的话继续往前
		} while (p != initPos);

		// Decompress coordinates and convert back to 3-space
		ll time = -1;
		pnt3 prev = {};
		//计算robot运行轨迹，将segs的轨迹还原为3维坐标
		for (const pnt2 &p : segs){
			int x = coords[0][p[0]];
			int y = coords[1][p[1]];
			pnt3 q = initDirection * x + initFace * y;
			q[axisIgnored] = robotInitPos[axisIgnored];
			paths[i].push_back(q);
			if (time == -1)
				time = 0;
			else
				time += distance(prev, q);
			times[i].push_back(time);
			prev = q;
		}
		int a=1;
	}

#if 0
	for (int i = 0; i < K; i++)
    {
        cerr << "Robot " << i << '\n';
        for (const auto &p : paths[i])
            cerr << '(' << p[0] << ", " << p[1] << ", " << p[2] << ")\n";
    }
#endif

	ll ans = LLONG_MAX;
	ll hits = 0;
	//每两条轨迹求交
	for (int i = 0; i < K; i++)
		for (int j = i + 1; j < K; j++){
			//轨迹中的每一段拎出来互相判断是否碰撞(相交)
			for (size_t u = 0; u + 1 < paths[i].size(); u++){
				pnt3 ai = paths[i][u];
				pnt3 bi = paths[i][u + 1];
				pnt3 di = direction(ai, bi);
				//轨迹都是平行于坐标轴的,所以两个坐标只有一维不同
				bool upi = ai[0] + ai[1] + ai[2] < bi[0] + bi[1] + bi[2];
				pnt3 li = upi ? ai : bi;
				pnt3 hi = upi ? bi : ai;
				for (size_t v = 0; v + 1 < paths[j].size(); v++){
					pnt3 aj = paths[j][v];
					pnt3 bj = paths[j][v + 1];
					pnt3 dj;
					int dotp;
					for (int k = 0; k < 3; k++){
						if (li[k] > aj[k] && li[k] > bj[k])
							goto skip;
						if (hi[k] < aj[k] && hi[k] < bj[k])
							goto skip;
					}
					dj = direction(aj, bj);
					dotp = dot(di, dj);//dotp只可能等于0,1,-1
					if (dotp < 0){
						// Parallel and in opposite directions
						ll separateDistance = dot(aj, di) - dot(ai, di);
						ll t = (separateDistance + times[i][u] + times[j][v]) / 2;
						if (t >= times[i][u] && t <= times[i][u + 1] &&
							t >= times[j][v] && t <= times[j][v + 1]){
							hits++;
							ans = min(ans, t);
						}
					} else if (dotp == 0){
						ll ti = times[i][u] + dot(aj, di) - dot(ai, di);
						ll tj = times[j][v] + dot(ai, dj) - dot(aj, dj);
						auto solve = crt(ti, times[i].back(), tj, times[j].back());
						if (solve.second > 0){
							hits++;
							ans = min(ans, solve.first);
						}
					}

					skip:;
				}
			}
		}
	cerr << "hits: " << hits << '\n';
	if (ans == LLONG_MAX)
		cout << "ok\n";
	else
		cout << ans / 2 << '\n';
}
