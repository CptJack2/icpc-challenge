#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	int P, H;
	while (cin >> P >> H) {
		vector<vector<int>> S(P, vector<int>(H));
		vector<int64_t> tot(P);
		for (int i = 0; i < P; i++) {
			for (int j = 0; j < H; j++) {
				cin >> S[i][j];
				tot[i] += S[i][j];
			}
			S[i].push_back(0);//设置一个最小的地板，在26行的循环递增条件用
			sort(S[i].begin(), S[i].end(), greater<int>());
		}

		for (int i = 0; i < P; i++) {
			vector<pair<int, int>> events;
			int cur = 0;
			for (int j = 0; j < P; j++) {
				int64_t itot = tot[i], jtot = tot[j], lim = 1000000000;
				if (jtot <= itot) cur++;
				for (int ih = 0, jh = 0; ih < H || jh < H; S[i][ih] > S[j][jh] ? ih++ : jh++) {
					bool old = (jtot <= itot);//这里的条件比较巧妙，具体看下面
					int v = max(S[i][ih], S[j][jh]);//不断取ij的scores中较大者为lim,如果有重复,当两个都到达小值时才会进入下一lim
					itot -= (lim-v) * ih; jtot -= (lim-v) * jh;//对两者总score进行裁减
					lim = v;
					if (!old && jtot <= itot) {
						/*排名要+1，在前一lim需要jtot>itot（=不行，因为排名算的是小于等于自己分数的个数），当前lim jtot <= itot
						 *直线过（lim，tot）点，斜率是h，分别列出ij两直线的方程式，求出交点横坐标就是lim+(itot-jtot)/(jh-ih)
						 *判断条件相当于if(orig_jtot>orig_itot && (jtot<itot || jtot==itot))*/
						events.emplace_back(lim+(itot-jtot)/(jh-ih), 1);
					} else if (old && jtot > itot) {
						/*排名要-1，在前一lim需要jtot<=itot，在当前lim jtot>itot，并且-1的效果要计入交点的向下取整的点，所以会有jtot-itot-1
						 *判断条件相当于if((orig_jtot>orig_itot || orig_jtot==orig_itot) && jtot>itot)*/
						events.emplace_back(lim+(jtot-itot-1)/(ih-jh), -1);
					}
				}
			}

			sort(events.begin(), events.end(), greater<pair<int, int>>());
			int ret = cur;
			for (auto const& e : events) { cur += e.second; ret = min(ret, cur); }
			cout << ret << endl;
		}
	}
}