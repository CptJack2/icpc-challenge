#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vll = vector<ll>;

static constexpr int SCALE = 10000;

static int load_coord()
{
	double x;
	cin >> x;
	return int(round(x * SCALE));
}

static vll load_map(int N)
{
	vector<pii> points;
	for (int i = 0; i < N; i++)
	{
		int lat = load_coord();
		int lon = load_coord();
		points.emplace_back(lon, lat);
	}
	sort(points.begin(), points.end());
	vll out(N);
	ll plon = points.back().first - 360 * SCALE;
	for (int i = 0; i < N; i++)
	{
		out[i] = (points[i].first - plon) * 1000000000LL + points[i].second;
		plon = points[i].first;
	}
	return out;
}

int main()
{
	ios::sync_with_stdio(false);
	int N;
	cin >> N;
	vll map1 = load_map(N);
	vll map2 = load_map(N);
	vi fail(N + 1);
	fail[0] = -1;
	for (int i = 1; i <= N; i++)
	{
		ll s = map1[i - 1];
		int f = fail[i - 1];
		while (f >= 0 && map1[f] != s)
			f = fail[f];
		fail[i] = f + 1;
	}

	int m = 0;
	for (int i = 0; i < 2 * N; i++)
	{
		ll s = map2[(i >= N) ? i - N : i];
		while (m >= 0 && map1[m] != s)
			m = fail[m];
		m++;
		if (m == N)
		{
			cout << "Same\n";
			return 0;
		}
	}

	cout << "Different\n";
	return 0;
}
