#include "bits/stdc++.h"
using namespace std;

int main() {
	int P, H;
	cin>>P>>H;
	vector<vector<int>> scores(P, vector<int>(H));
	vector<long> sum(P,0);//洞数的总和最大为500*10^9,需要用long
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < H; j++) {
			cin >> scores[i][j];
			sum[i] += scores[i][j];
		}
		sort(scores[i].begin(),scores[i].end(),greater<int>());
	}

	for (int i = 0; i < P; i++) {
		int rank = 1;//自己的一个
		//map key为取的lim, value表示取此lim时,排名需要如何变化
		map<int,int,greater<int>> rankChange;
		for (int j = 0; j < P; j++) {
			if (j == i)
				continue;
			long isum = sum[i], jsum = sum[j];
			if (jsum <= isum)
				++rank;
			//以i号玩家和j号玩家的分数作为limit
			set<int, greater<int>> limits;
			for (auto l:scores[i])limits.insert(l);
			for (auto l:scores[j])limits.insert(l);
			limits.insert(999999999);//加一个无限制
			int iLimitedHoles = 0, jLimitedHoles = 0;
			for (auto it = limits.begin(); it != prev(limits.end()); ++it) {
				bool jLessEqual = (jsum <= isum);
				int lim = *next(it), prevLim = *it;
				while (lim < scores[i][iLimitedHoles] && iLimitedHoles<H)++iLimitedHoles;
				while (lim < scores[j][jLimitedHoles] && jLimitedHoles<H)++jLimitedHoles;
				isum -= long(prevLim - lim) * iLimitedHoles;//转成long防溢出
				jsum -= long(prevLim - lim) * jLimitedHoles;
				//排名要+1，在前一lim需要jsum>isum（=不行，因为排名算的是小于等于自己分数的个数），当前lim jsum <= isum
				//排名要-1，在前一lim需要jsum<=isum，在当前lim jsum>isum
				if (!jLessEqual && jsum <= isum || jLessEqual && jsum > isum) {
					//每一条分段直线过（lim，sum）点，斜率是LimitedHoles，分别列出ij两直线的方程式，求出交点横坐标就是lim+(isum-jsum)/(jLimitedHoles-iLimitedHoles)
					double intersectionPoint = lim + double(isum - jsum) / (jLimitedHoles - iLimitedHoles);
					//因为lim都是整数,在交点的向下取整位置记入更新
					int updatePoint = floor(intersectionPoint);
					//对于-1的效果,如果交点是整数,需要计入前一个点
					if (jLessEqual && jsum > isum && int(intersectionPoint) == intersectionPoint)
						--updatePoint;
					rankChange[updatePoint]+=!jLessEqual?1:-1;
				}
			}
		}
		int minRank = rank;
		for (auto const &r : rankChange) {
			rank += r.second;
			minRank = min(minRank, rank);
		}
		cout << minRank<< endl;
	}
}