/*∵ max(xi-x,0)+max(yi-y,0)+max(zi-z,0) = max(0, xi-x, yi-y, zi-z, xi+yi-x-y, xi+zi-x-z, yi+zi-y-z, xi+yi+zi-x-y-z)
 *
 * ∴ max(1<=i<=n)(max(xi-x,0)+max(yi-y,0)+max(zi-z,0)) =
 * max(
 *  0,
 * 	max(1<=i<=n)(xi-x),
 * 	max(1<=i<=n)(yi-y),
 * 	max(1<=i<=n)(zi-z),
 * 	max(1<=i<=n)(xi+yi-x-y),
 * 	max(1<=i<=n)(xi+zi-x-z),
 * 	max(1<=i<=n)(yi+zi-y-z),
 * 	max(1<=i<=n)(xi+yi+zi-x-y-z)
 *)=
 * max(
 *  0,
 * 	max(1<=i<=n)(xi)-x,
 * 	max(1<=i<=n)(yi)-y,
 * 	max(1<=i<=n)(zi)-z,
 * 	max(1<=i<=n)(xi+yi)-x-y,
 * 	max(1<=i<=n)(xi+zi)-x-z,
 * 	max(1<=i<=n)(yi+zi)-y-z,
 * 	max(1<=i<=n)(xi+yi+zi)-x-y-z
 *)
 * */
#include "bits/stdc++.h"
using namespace std;

int main(){
	int n;
	cin>>n;
	int v[3];
	vector<long[8]> partialSum(n);
	long maximums[8];
	memset(maximums,0,8*sizeof(long));
	long bestCost=LONG_MAX,ind=-1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < 3; ++j)
			cin>>v[j];
		for (int j = 0; j < 8; ++j) {
			long sum=0;
			if(j&0b1)sum+=v[0];
			if(j&0b10)sum+=v[1];
			if(j&0b100)sum+=v[2];
			maximums[j]=max(maximums[j],sum);
			partialSum[i][j]=sum;
		}
	}
	for (int i = 0; i < n; ++i) {
		long cost=0;
		for (int j = 0; j < 8; ++j)
			cost=max(cost,maximums[j]-partialSum[i][j]);
		if(bestCost>cost)
			bestCost=cost,
			ind=i;
	}
	cout<<bestCost<<" "<<ind+1<<endl;
}