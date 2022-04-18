#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	int N, V, C;
	while (cin >> N >> V >> C) {
		vector<pair<int, int>> DX(N);
		for (auto& [d, x] : DX) cin >> x >> d;

		vector<bool> dyn(N * 2000 + 2001);
		dyn[0] = true;
//		for (auto& [d, x] : DX) d = (d*V-1)/C + x;
		sort(DX.begin(), DX.end(),
	   		[&](const pair<int,int>& a,const pair<int,int>& b){
				long av=(long)C*a.first+a.second*V, bv=(long)C*b.first+b.second*V;
				return av<bv;
			});
		int totx = 0;
		for (auto [d, x] : DX) {
			vector<int> dynt;
			int t=(d*V-1)/C;
			int oi = min(totx, t);
			for (int i=oi; i >= 0; i--)
				if (dyn[i])
//					dynt.push_back(i),
					dyn[i+x] = true;
			totx += x;
		}

		int64_t ret = totx;
		for (int64_t i = totx; ; i--)
			if (dyn[i]) {
				ret += i * (C-1);
				break;
			}

		cout << ret << endl;
	}
}

