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

long double k[V+5];
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
	for(int i=0;i<P;++i){
		int ii,j;
		for(ii=1; ii <= V; ++ii)
			k[ii]=1;
		memset(vis,0,sizeof(vis));//初始化
		int x,w;
		long double o=1;
		for(ii=1; ii <= n && o > 1e-12 ; ii++) {
			w = T(p[ii] / gcd(P, p[ii]));
			o /= k[w];
			for ( j = 0; j < w; ++j) {
				x = (j * P + i) % p[ii];
				if(tg[ii][x] && !vis[w][j]){
					//枚举时间jP+i
					k[w] -= 1.0 / w;
					vis[w][j] = 1;
				}
			}
			o*=k[w];
			f[ii]+=o;
		}
	}
	for(int i=1;i<=n+1;++i)
		printf("%.12Lf\n",(f[i-1]-f[i])/P);
	return 0;//枚举模P余数，最终答案要除以P
}
