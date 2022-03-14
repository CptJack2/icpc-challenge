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
	/*一路绿灯到第i盏灯的余数数量
	 * 每一盏灯，各有多少个余数能通过，除以总的余数数（magicNumber），就是这盏灯的通过概率*/
	vector<long double> passModNum(n + 1);
	//第一盏灯所有的余数都能通过
	passModNum[0]=magicNumber;
	/*当取定一个mod,以mod+k*magicNumber作为出发时间点,对于每一盏灯,
	 * f_i(k)=light_i(mod+k*magicNumber)(到达路灯时路灯的颜色),f_i(k)是一个周期函数
	 * 并且每两个函数的周期都是互质或者成倍数的，周期互质的概率可以直接相乘，成倍数的可以简化成相同的周期*/
	for(int mod=0; mod < magicNumber; ++mod){
		vector<int> periodPassed(maxPeriod);
		for(int i=1; i <= maxPeriod; ++i)
			periodPassed[i]=i;
		//第一维是周期大小,第二维指示这个周期下的某一个magicNumber的倍数是否在前面的路灯被挡掉了
		vector<vector<bool>> filtered(maxPeriod, vector<bool>(maxPeriod, false));
		//这个余数一直走到头的概率，不断更新
		long double modPassProbability=1;
		for(int i=1; i <= n; i++) {
			//f_i(k)的周期
			int newPeriod = periods[i] / gcd(magicNumber, periods[i]);
			//质数幂次的周期需要扩展到最大
			if(newPeriod==2 || newPeriod==4)
				newPeriod=8;
			else if(newPeriod==3)
				newPeriod=9;
			int passedBefore=periodPassed[newPeriod];
			//如果到了这个灯已经没有车可以过来了,必须停,否则后面会拿0做除数算出个无穷大的概率
			if(passedBefore==0)
				break;
			//对于周期函数f_i(k)，尝试一个周期内所有的k，看看哪些可以通过
			for (int k = 0; k < newPeriod; ++k) {
				int startTime = (k * magicNumber + mod) % periods[i];
				if(modsCantPass[i][startTime] && !filtered[newPeriod][k]){
					--periodPassed[newPeriod];
					filtered[newPeriod][k] = true;
				}
			}
			//通过这盏灯后的倍数的数目，除以通过这盏灯前的倍数的数目，就是能够通过这盏灯的概率
			modPassProbability*= (long double)(periodPassed[newPeriod]) / passedBefore;
			//由于每个余数的子问题都是独立的，所有余数通过这盏灯的概率的和，就是通过灯的总概率
			passModNum[i]+=modPassProbability;
		}
	}
	for(int i=1;i<=n;++i)
		cout << setprecision(12) << (passModNum[i - 1] - passModNum[i]) / magicNumber << endl;
	//一路绿灯的概率
	cout << setprecision(12) << passModNum[n] / magicNumber;
	return 0;
}
