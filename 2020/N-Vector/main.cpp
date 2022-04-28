#include "bits/stdc++.h"
using namespace std;

int main(){
	int d,n;
	cin>>d>>n;
//	vector<vector<double>> vecs(n,vector<double>(d));
//	vector<double> dists(n);
//	for(int i=0;i<n;++i) {
//		for (int j = 0; j < d; ++j)
//			cin>>vecs[i][j];
//		cin>>dists[i];
//	}
	vector<double> c1(d,0),c2(d,0);//两个圆心
	double r1,r2;//两个半径
	for (int j = 0; j < d; ++j)
		cin>>c1[j];
	cin>>r1;
	auto euclidDist=[&](const vector<double>& v1,const vector<double>& v2){
		double ret=0;
		for(int i=0;i<v1.size();++i)
			ret+=v1[i]*v1[i]+v2[i]*v2[i];
		return sqrt(ret);
	};
	vector<double> inter1,inter2;//两圆两个交点
	for(int i=1;i<n;++i){
		for (int j = 0; j < d; ++j)
			cin>>c2[j];
		cin>>r2;
		double d=euclidDist(c1,c2);
		if(d<abs(r1-r2))
			//缩小圆1
			if(r1>r2)
				swap(c1,c2),
				r1=r2;
		else if(r1+r2<d){
			vector<double> dir(d,0);
			for (int j = 0; j < d; ++j)
				dir[j]=c2[i]-c1[i];
			double ldir=euclidDist(vector<double>(d,0),dir);
			for (int j = 0; j < d; ++j) {
				cout<<
				}
		} else if(r1+r2==d){
			vector<double> dir(d, 0);
			for (int j = 0; j < d; ++j)
				dir[j] = c2[i] - c1[i];
			double ldir = euclidDist(vector<double>(d, 0), dir);
			for (int j = 0; j < d; ++j)
				cout << c1[j]+dir[j]/ldir*r1<<" ";
			return 0;
		}

	}
	return 0;
}