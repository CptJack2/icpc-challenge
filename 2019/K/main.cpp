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

inline void Solve(const int & t)/*余数为t时*/ {
 	int i,j;
	for(i=1;i<=V;++i) 
		k[i]=1;
	memset(vis,0,sizeof(vis));//初始化
 	int x,w;
	long double o=1;
	++f[0];
	for(i=1; i<=n && o>1e-12 ;i++) {
		for (o /= k[w = T(p[i] / gcd(P, p[i]))], j = 0; j ^ w; ++j) {
			x = (j * P + t) % p[i];
			if(tg[i][x] &&!vis[w][j]){
				//枚举时间jP+t
				k[w] -= 1.0 / w;
				vis[w][j] = 1;
			}
		}
		o*=k[w];
		f[i]+=o;
	}
}
int main(){
	int x,r;
	cin>>n;
	for(int i=1;i<=n;++i) {
		cin>>x>>r>>p[i];
		p[i]+=r;
		x%=p[i];
		for (int j = 0; j < r; ++j) {
			int ttt=(j-x+p[i])%p[i];
			tg[i][ttt]=1;//给红灯对应余数打标记
		}
	}
	for(int i=0;i!=P;++i)
		Solve(i);
	for(int i=1;i<=n+1;++i)
		printf("%.12Lf\n",(f[i-1]-f[i])/P);
	return 0;//枚举模P余数，最终答案要除以P
}
