#include<bits/stdc++.h>
using namespace std;

const int maxPeriod=100,
	magicNumber=2520;

inline int gcd(const int & x,const int & y) {return y?gcd(y,x%y):x;}

int main(){
	int n;
	cin>>n;
	vector<int> periods(n+1);
	//哪些出发时间的余数不能通过这盏红灯
	vector<vector<bool>> modsCantPass(n + 1);
	for(int i=1;i<=n;++i) {
		int x,r,g;
		cin >> x >> r >> g;
		periods[i]=r+g;
		x%=periods[i];
		modsCantPass[i]=vector<bool>(periods[i], false);
		for (int j = 0; j < r; ++j) {
			//假设起始时刻为s，令pi=ri+gi，那么不能通过第i个红绿灯就意味着(s+xi)%pi<ri。
			int s= (j - x + periods[i]) % periods[i];
			modsCantPass[i][s]=true;
		}
	}
	vector<long double> passProbability(n+1);
	//第一盏所有的都能通过
	passProbability[0]=magicNumber;
	for(int mod=0; mod < magicNumber; ++mod){
		vector<int> periodPassed(maxPeriod);
		for(int i=1; i <= maxPeriod; ++i)
			periodPassed[i]=i;
		//第一维是周期大小,第二维指示这个周期下的某一个magicNumber的倍数是否在前面的路灯被挡掉了
		vector<vector<bool>> filtered(maxPeriod, vector<bool>(maxPeriod, false));
		//这个余数一直走到头的概率，不断更新
		long double modPassProbability=1;
		for(int i=1; i <= n; i++) {
			//缩减的周期
			int newPeriod = periods[i] / gcd(magicNumber, periods[i]);
			//质数幂次的周期需要扩展到最大
			if(newPeriod==2 || newPeriod==4)
				newPeriod=8;
			if(newPeriod==3)
				newPeriod=9;
			int passedBefore=periodPassed[newPeriod];
			//如果到了这个灯已经没有车可以过来了,必须停,否则后面会拿0做除数算出个无穷大的概率
			if(passedBefore==0)
				break;
			for (int j = 0; j < newPeriod; ++j) {
				int startTime = (j * magicNumber + mod) % periods[i];
				if(modsCantPass[i][startTime] && !filtered[newPeriod][j]){
					--periodPassed[newPeriod];
					filtered[newPeriod][j] = true;
				}
			}
			modPassProbability*= (long double)(periodPassed[newPeriod]) / passedBefore;
			passProbability[i]+=modPassProbability;
		}
	}
	//枚举模P余数，最终答案要除以P
	for(int i=1;i<=n;++i)
		cout<<setprecision(12)<<(passProbability[i - 1] - passProbability[i]) / magicNumber<<endl;
	//一路绿灯的概率-
	cout<<setprecision(12)<<passProbability[n]/ magicNumber;
	return 0;
}
