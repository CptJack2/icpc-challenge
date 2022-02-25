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
			int itot = tot[i], jtot = tot[j];
			if (jtot <= itot) cur++;
			if(i==j)continue;
			set<int,greater<int>> limits;
			for(auto l:scores[i])
				limits.insert(l);
			for(auto l:scores[j])
				limits.insert(l);
			limits.insert(1000000000);
			int iholes=0,jholes=0;
			for (auto it=limits.begin();it!=prev(limits.end());++it) {
				int interval_end=*it, interval_start=*next(it);
				while(scores[i][iholes]>interval_start)++iholes;
				while(scores[j][jholes]>interval_start)++jholes;
				bool end_i_bigger = (jtot < itot);
				itot -= (interval_end - interval_start) * iholes;
				jtot -= (interval_end - interval_start) * jholes;
				bool start_i_bigger = (jtot < itot);
				auto add_events=[&](double pos,int v){
					if(pos==1)return;
					if(!events.empty() && events.back().first== pos)
						events.back().second+=v;
					else
						events.emplace_back(pos,v);
				};
				if (iholes != jholes && start_i_bigger ^ end_i_bigger){
//					double intersection_point= interval_start + double(itot - jtot) / (jholes - iholes);
//					if(intersection_point - interval_start > 1)
//						events.emplace_back(intersection_point, end_i_bigger ? -1 : 1);
					add_events(interval_start + double(itot - jtot) / (jholes - iholes), end_i_bigger ? -1 : 1);
				}
				else if(iholes==jholes && itot==jtot) //区间内曲线重叠
					add_events(interval_end,1);
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