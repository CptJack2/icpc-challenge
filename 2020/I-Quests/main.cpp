#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	int n, v, c;
	cin>>n>>v>>c;
	vector<pair<int, int>> xd(n);
	for(int i=0;i<n;++i)
		cin >> xd[i].first >> xd[i].second;
	/* 三条原则:
	 * 1.应该先做能拿到bonus的quest,后做拿不到bonus的,即最后完成任务的序列,前半部分是有bonus quest,后本部分是无bonus quest:
	 * 假设在前面做了一个无bonus quest q1,后面做了一个有bonus q2. q1 q2交换顺序,q1仍无bonus;q2因为提前了,肯定还有bonus,所以不影响总分
	 * 对一个能得到最大得分的任务序列中每一对这样的q1q2同样操作,总能得到一个前半部分是有bonus quest,后本部分是无bonus quest的序列
	 * 所以求最大得分序列的时候可以遵循这条原则
	 *
	 * 2.无bonus的quest,完成的顺序不影响
	 * 因为无论怎么样都是拿基础分
	 *
	 * 3.设完成这个任务后最大可能达到的分数maxScore(i)=c*xi+v*di-1,maxScore小的quest应该先做
	 * 根据原则1,如果有一个能赶上deadline,另一个不能,能赶上的应该排前面,
	 * 根据原则2,如果q1q2都不能在deadline内完成,讨论它们的顺序无意义,
	 * 那么设q1q2顺序相邻且都能拿到bonus,设mS1>mS2
	 * 有一个隐含推断,设当前分数为sco, sco+c*x<maxScore=c*x+v*d-1 等价于 sco<v*d-1, 即获得bonus完成任务的情况下,分数小于maxScore,则当前分数肯定小于任务的等级限制
	 * 将q2提前,q2肯定还能拿到bonus; q1放后,因为q1q2做完总分<=mS2<mS1,所以q2q1做完的总分不变,还是小于mS1,所以根据上面的隐含推断,变换后的顺序仍合法
	 * 同样可以像原则1中的操作方式,得到一个最大得分任务序列满足原则3*/

	//先按deadline对quest排序
	sort(xd.begin(), xd.end(),
		 [&](const pair<int, int> &a, const pair<int, int> &b) {
			 long av = c * a.first + a.second * v, bv = c * b.first + b.second * v;
			 return av < bv;
		 });
	int baseScoreTotal = 0;//未计bonus的总得分
	/* 首先每一个任务都能得到基础的xi分,接下来要使总得分最大,
	 * 称能得到bonus的任务的集合为bonus集合,
	 * 只需要bonus集合内的任务总分为最大即可,
	 * 通过动态规划的方法尝试将不同的任务加入bonus集合，dp数组的下标是bonus集合的总分*/
	//这里不用set是因为O(N)的更新跑不掉了，vector<bool>的实现就是一个bitset。size=n*max(v)+1
	vector<bool> dp(n * 2000 + 1,false);
	dp[0] = true;
	int maxBonus=0;
	for(auto[x,d] : xd) {
		//当前quest能加入bonus集合的充要条件,当前总分<=d*v-1,由于bonus任务先做,所以总分等于bonus集合内已有任务的总分
		int maxBonusSetScore = (d * v - 1) / c;
		for (int i = min(baseScoreTotal, maxBonusSetScore); i >= 0; i--)
			if (dp[i])
				dp[i + x] = true,
				maxBonus=max(maxBonus,i+x);
		baseScoreTotal += x;
	}
	//基础得分+最大bonus
	cout<<baseScoreTotal+maxBonus*(c-1);

	return 0;
}

