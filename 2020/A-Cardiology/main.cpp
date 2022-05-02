#include "bits/stdc++.h"
using namespace std;

long r,c;
int main(){
	cin>>r>>c;
	struct ansStru{
		pair<long,long> rc;
		int iterationTimes;
	};
	vector<ansStru> stablePos(c);
	//遍历每一个可能的列p
	for (long p = 0; p < c; ++p) {
		//下标从0开始
		pair<long,long> start={0,0},end={r-1,c-1};//目标卡片可能处于的位置的范围
		int iterationTime=0;
		//第p列开始和结束卡片的序号
		pair<long,long> pthStart={r*p/c,r*p%c},
			pthEnd={(r*p+c-1)/c,(r*p+c-1)%c};
		while(1){
			//计算一个行列坐标往后数v个之后的坐标
		    auto rcAdd=[&](const pair<long,long>& rc,int v)->pair<long,long>{
				auto ret=rc;
				ret.first+=v/c;
				v%=c;
				ret.second+=v;
				if(ret.second>=c)
					ret.second%=c,
					++ret.first;
				return ret;
			};
			auto oS=start,
				oE=end;
			//随着卡片所在列每次都是第p位被选择,卡片可能处于的位置不断收缩
			start=rcAdd(pthStart,start.first);
			end=rcAdd(pthStart,end.first);
			//直到可能范围不变化,就结束迭代
			if(oS==start && oE==end)
				break;
			++iterationTime;
		}
		//可能范围唯一才记录解
		stablePos[p].rc=(start == end ? start : pair<long,long>{-1, -1});
		stablePos[p].iterationTimes=iterationTime;
	}
	int ans,
		minDist=999999999;
	//计算离行列中心的距离
	auto calDist=[&](const pair<long,long> rc)->long{
		const int mr=r/2, mc=c/2;
		long minr=abs(rc.first-mr),
			minc=abs(rc.second-mc);
		if(r%2==0)
			minr=min(minr,abs(rc.first-mr+1));
		if(c%2==0)
			minc=min(minc,abs(rc.second-mc+1));
		return minr+minc;
	};
	//选取离中心最小距离的解
	for (int i = 0; i < stablePos.size(); ++i) {
		auto pi=stablePos[i].rc;
		auto dist=calDist(pi);
		if(pi!=pair<long,long>{-1,-1} && dist<minDist)
			minDist=dist,
			ans=i;
	}
	cout<<ans+1<<" "<<
	stablePos[ans].rc.first+1<<" "<<stablePos[ans].rc.second+1<<" "<<
	stablePos[ans].iterationTimes;
}