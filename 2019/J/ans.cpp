#include "bits/stdc++.h"
using namespace std;

int main() {
	int P, H;
	cin >> P >> H;
	vector<vector<int>> scores(P, vector<int>(H));
	vector<int> tot(P);
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < H; j++) {
			cin >> scores[i][j];
			tot[i] += scores[i][j];
		}
		sort(scores[i].begin(), scores[i].end(), greater<int>());
	}

	for (int i = 0; i < P; i++) {
		vector<pair<double, int>> events;//intersection point
		int cur = 0;
		for (int j = 0; j < P; j++) {
			int itot = tot[i], jtot = tot[j], interval_end = 1000000000;
			if (jtot <= itot) cur++;
			if(i==j)continue;
			set<int,greater<>> limits;
			for(auto l:scores[i])
				limits.insert(l);
			for(auto l:scores[j])
				limits.insert(l);
			limits.insert(interval_end);
			for (int iholes = 0, jholes = 0; iholes < H || jholes < H; scores[i][iholes]>scores[j][jholes]?++iholes:++jholes) {
				int interval_start =scores[i][iholes] > scores[j][jholes] ? scores[i][iholes] : scores[j][jholes];
				bool end_i_bigger = (jtot < itot);
				itot -= (interval_end - interval_start) * iholes;
				jtot -= (interval_end - interval_start) * jholes;
				bool start_i_bigger = (jtot < itot);
				if (iholes != jholes && start_i_bigger ^ end_i_bigger)
					events.emplace_back(interval_start + double(itot - jtot) / (jholes - iholes), end_i_bigger ? -1 : 1);
				else if(iholes==jholes && itot==jtot) //区间内曲线重叠
					events.emplace_back(interval_end,1);
				interval_end = interval_start;
			}
		}
		sort(events.begin(), events.end(), greater<pair<double, int>>());
		int ret = cur;
		for (auto const &e : events) {
			if(e.second==1){
				cur += e.second;
				ret = min(ret, cur);
			} else{
				ret = min(ret, cur);
				cur += e.second;
			}

		}
		cout << ret << endl;
	}
}