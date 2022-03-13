#include<bits/stdc++.h>
#define N 500
#define V 100
#define P 2520
using namespace std;

int n,p[N+5],tg[N+5][V+5];long double f[N+5];

inline int gcd(const int & x,const int & y) {return y?gcd(y,x%y):x;}

inline int T(const int & x) {
	return x==2||x==4?8:(x==3?9:x);
}//把x调到可能的最大倍数

//long double k[V+5];
int vis[V+5][V+5];

int main(){
	int x,r;
	cin>>n;
	for(int i=1;i<=n;++i) {
		cin>>x>>r>>p[i];
		p[i]+=r;
		x%=p[i];
		for (int j = 0; j < r; ++j) {
			//假设起始时刻为s，令pi=ri+gi，那么不能通过第i个红绿灯就意味着(s+xi)%pi<ri。
			int s= (j - x + p[i]) % p[i];
			tg[i][s]=1;//给红灯对应余数打标记
		}
	}
	f[0]=P;
	for(int remain=0; remain < P; ++remain){
		int i,j;
		int k[V];
		for(i=1; i <= V; ++i)
			k[i]=i;
		memset(vis,0,sizeof(vis));//初始化
		int x,multiple;
		long double passPossiblity=1;
		for(i=1; i <= n && passPossiblity > 1e-12 ; i++) {
			multiple = T(p[i] / gcd(P, p[i]));//缩减的周期
			int oldK=k[multiple];
			for ( j = 0; j < multiple; ++j) {
				x = (j * P + remain) % p[i];
				if(tg[i][x] && !vis[multiple][j]){
					//枚举时间jP+remain
					k[multiple] --;
					vis[multiple][j] = 1;
				}
			}
			passPossiblity*= (long double)(k[multiple]) / oldK;
			f[i]+=passPossiblity;
		}
	}
	for(int i=1;i<=n+1;++i)
		printf("%.12Lf\n",(f[i-1]-f[i])/P);
	return 0;//枚举模P余数，最终答案要除以P
}
