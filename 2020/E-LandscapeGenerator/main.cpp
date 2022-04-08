#include "bits/stdc++.h"
using namespace std;

int main(){
	int n,k;
	cin>>n>>k;
	//slope定义为右边斜率,比如H(x1,x2), x1斜率就为1,中点斜率为0/-1,x2斜率为0
	vector<int>diff(n+2,0), diffSlope(n+1,0);
	for(int i=0;i<k;++i){
		char op;
		int x1,x2;
		cin>>op>>x1>>x2;
		auto addSlope=[&](int sl){
			if(x1==x2-1)return;
			int mid=(x1+x2)/2;
			diffSlope[x1]+=sl;
			if((x2-x1)%2==0)
				diffSlope[mid]-=2*sl;
			else
				diffSlope[mid]-=sl,
				diffSlope[mid+1]-=sl;
			diffSlope[x2]+=sl;
		};
		auto addHeight=[&](int h){
			diffSlope[x1]+=h;
			diffSlope[x2+1]-=h;
		};
		switch (op) {
		case 'H':
			addHeight(1);
			addSlope(1);
			break;
		case 'V':
			addHeight(-1);
			addSlope(-1);
			break;
		case 'R':
			addHeight(1);
			break;
		case 'D':
			addHeight(-1);
			break;
		}
	}
	vector<int> ret(n+1,0);
	int slope=diffSlope[1];
	for (int i = 2; i <= n; ++i) {
		ret[i]=ret[i-1]+slope;
		slope+=diffSlope[i];
	}
	int dif=diff[1];
	for (int i = 1; i <= n; ++i) {
		ret[i]+=dif;
		cout<<ret[i]<<endl;
		dif+=diff[i+1];
	}
	return 0;
}