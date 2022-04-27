#include "bits/stdc++.h"
using namespace std;

//扩展欧几里得算法，返回gcd(a,b)，并得到(x,y)，使得ax+by=gcd(a,b)
int gcdEx(int a,int b,int*x,int*y)
{
	if(b==0)
	{
		*x=1,*y=0 ;
		return a ;
	}
	else
	{
		int r=gcdEx(b,a%b,x,y);
		/* r = GCD(a, b) = GCD(b, a%b) */
		int t=*x ;
		*x=*y ;
		*y=t-a/b**y ;
		return r ;
	}
}

//求解同余方程a*x≡m(mod b)
int solveCongruenceEquation(int a,int b,int m){
	int x,y;
	int d= gcdEx(a,b,&x,&y);
	//m不是gcd(a,b)的整数倍，方程无整数解
	if(m/d!=(double)m/d)
		return -1;
	int t=(a*b/d);
	//将x调整为正数
	return (x*m/d%t+t)%t;
}

int main(){
	int b,d;
	vector<int>a;
	cin>>b>>d;
	while (!cin.eof()){
		char c;
		cin>>c;
		a.push_back(c-'0');
	}
	int l=a.size();
	vector<int> ten2LMod(l+1);//10^l对b的mod
	vector<int> dMod(l+1);//l个d的mod
	ten2LMod[0]=1;
	dMod[0]=0;
	for (int i = 1; i <=l ; ++i)
		ten2LMod[i]=ten2LMod[i-1]*10%b,
		dMod[i]=(dMod[i-1]*10+d)%b;
	//大数mddddd...是否小于等于a
	auto leA=[&](int m,int l){
		vector<int> mm;
		while(m)
			mm.push_back(m%10),
			m/=10;
		if(l+mm.size()>a.size())
			return false;
		if(l+mm.size()<a.size())
			return true;
		for(int i=a.size()-1;i>=0;--i) {
			int di=i>=l?mm[i-l]:d;
			if (a[i] > d)
				return true;
			if(a[i]<d)
				return false;
			if(i==0)
				return true;
		}
	};
	//求解同余方程10^l*x+ddd...≡0(mod b)，即ten2LMod[i]*x≡-dMod[i](mod b)
	for (int i = l; i >=0 ; --i) {
		int x= solveCongruenceEquation(ten2LMod[i],b,-dMod[i]);
		if(leA(x,i)) {
			cout << i;
			return 0;
		}
	}
}