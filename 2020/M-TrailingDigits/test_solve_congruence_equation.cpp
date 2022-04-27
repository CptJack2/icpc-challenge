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
	//将x调整为正数
	return (x*m/d%b+b)%b;
}

int main(int argc, char** argv){
	int a=atoi(argv[1]),
		b=atoi(argv[2]),
		m=atoi(argv[3]);
	int x=solveCongruenceEquation(a,b,m);
	cout<< x<<endl;
	cout<< (a*x%b==m%b? "succ":"fail") << endl;
}