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
pair<int,int> solveCongruenceEquation(int a,int b,int m){//返回解和gcd(a,b)
	int x,y;
	int d= gcdEx(a,b,&x,&y);
	//m不是gcd(a,b)的整数倍，方程无整数解
	if(m/d!=(double)m/d)
		return {-1,-1};
	//将x调整为正数
	x=(x*m/d%b+b)%b;
	return {x, d};
}

int main(){
	int b,d;
	vector<int>a;
	cin>>b>>d;
	while (!cin.eof() && cin.peek()!='\n'){
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
		reverse(mm.begin(),mm.end());
		if(l+mm.size()>a.size())
			return false;
		if(l+mm.size()<a.size())
			return true;
		for(int i=0;i<a.size();++i) {
			int di=i<mm.size()?mm[i]:d;
			if (a[i] > di)
				return true;
			if(a[i]<di)
				return false;
			if(i==a.size()-1)
				return true;
		}
	};
	int ans=0,iLB=0,lenb=0,tb=b;
	while(tb)++lenb,tb/=10;//计算b的长度
	for (int i = l; i >= iLB ; --i) {
		//求解同余方程10^l*x+ddd...≡0(mod b)，即ten2LMod[i]*x≡-dMod[i](mod b)
		auto [x,gcd]= solveCongruenceEquation(ten2LMod[i],b,-dMod[i]);
		if(x==-1)continue;
		//0不能做数字开头
		if(x==0)x+=b/gcd;
		while(x<=b) {//x+k*b/gcd都是方程的解
			if (leA(x, i)) {
				//让后缀尽量长
				int cnt=0,tx=x;
				while (tx && tx % 10 == d)
					++cnt, tx /= 10;
				ans=max(ans,i+cnt);
				iLB= max(i - lenb,0);//再往后算lenb个,能保证长度一定不超过现在的
			}else
				break;
			x+=b/gcd;
		}
	}
	cout<<ans;
	return 0;
}