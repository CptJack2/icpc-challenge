#include"bits/stdc++.h"
using namespace std;

const long inf=0x4fffffffffffffff;
int n,h,alpha,beta1;

int main(){
	cin >> n >> h >> alpha >> beta1;
	vector<pair<int,int>> data(n);
	//dp方法求解，数组内为从点0到i点之间建造桥梁最小代价
	vector<long> dp(n,inf);
	for(int i=0;i<n;i++){
		cin>>data[i].first>>data[i].second;
	}
	//0点先盖个柱子
	dp[0]=alpha*(h-data[0].second);
	for(int i=0;i<n-1;i++){
		//初始化点i能建造的最大arch半径，
		long minDist=0,maxDist=2*(h-data[i].second);
		for(int j=i+1;j<n;j++){
			//两点之间的跨度
			long dist=data[j].first-data[i].first;
			//j点的柱子高度
			long pillar_h=h-data[j].second;
			//如果i点的桥下空间已经不足以支撑建造跨度这么远的拱，后面的也不用继续算了
			if(dist>maxDist)
				break;
			/*这里先设圆的半径为R，设i点x坐标为0，d为ij间水平距离，h是j处桥下的空间
			 * 那么圆心坐标就是（R，H-R), j点坐标是（d,H-h)，临界条件就是j点坐标刚好在这个半径R的圆上
			 * 对应方程(R-d)^2+(R-h)^2=R^2，一元二次方程的通解公式解出来就可以*/
			//半圆的左半边刚好擦过j点的直径
			maxDist=min(maxDist,2*dist+2*pillar_h+long(sqrt(8*dist*pillar_h)));
			//如果j高度不足建造拱桥，需要求出半圆的右半边刚好擦过j点的直径
			if(2*pillar_h<=dist)
				minDist=max(minDist,2*dist+2*pillar_h-long(sqrt(8*dist*pillar_h)));
			if(maxDist< minDist)
				break;
			if(dist>maxDist || dist<minDist)
				continue;
			long cost= dist * dist * beta1 + alpha * (h - data[j].second);
			if(cost+dp[i]<dp[j])
				dp[j]=cost+dp[i];
		}
	}
	if(dp[n-1]<inf)
		cout<<dp[n-1]<<endl;
	else
		cout<<"impossible"<<endl;
}