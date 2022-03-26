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
	for (long p = 0; p < c; ++p) {
		//下标从0开始
		pair<long,long> start={0,0},end={r-1,c-1};
		int iterationTime=0;
		pair<long,long> pthStart={r*p/c,r*p%c},
			pthEnd={(r*p+c-1)/c,(r*p+c-1)%c};
		while(1){
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
			start=rcAdd(pthStart,start.first);
			end=rcAdd(pthStart,end.first);
			if(oS==start && oE==end)
				break;
			++iterationTime;
		}
		stablePos[p].rc=(start == end ? start : pair<long,long>{-1, -1});
		stablePos[p].iterationTimes=iterationTime;
	}
	int ans,
		minDist=999999999;
	auto calDist=[&](const pair<long,long> rc)->long{
		const int mr=r/2, mc=c/2;
		long minr=abs(rc.first-mr),
			minc=abs(rc.second-mc);
		if(r%2)
			minr=min(minr,abs(rc.first-mr+1));
		if(c%2)
			minc=min(minc,abs(rc.second-mc+1));
		return minr+minc;
	};
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