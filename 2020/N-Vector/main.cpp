#include "bits/stdc++.h"

using namespace std;

const double eps=1e-6;

vector<double> operator-(const vector<double>& v1, const vector<double>& v2){
	vector<double> ret(v1.size());
	for(int i=0;i<v1.size();i++){
		ret[i]=v1[i]-v2[i];
	}
	return ret;
}

vector<double> operator/(const vector<double>& v,double c){
	vector<double> r(v.size());
	for(int i=0;i<v.size();i++){
		r[i]=v[i]/c;
	}
	return r;
}

double length(const vector<double>& v){
	double r=0;
	for(auto e:v)r+=e*e;
	return sqrt(r);
}

double operator*(const vector<double>& v1, const vector<double> & v2){
	double r=0;
	for(int i=0;i<v1.size();i++){
		r+=v1[i]*v2[i];
	}
	return r;
}

vector<double> operator*(double c, const vector<double>& v){
	vector<double> r(v.size());
	for(int i=0;i<v.size();i++){
		r[i]=v[i]*c;
	}
	return r;
}

vector<double> operator+(const vector<double>& v1, const vector<double> & v2){
	vector<double> r(v1.size());
	for(int i=0;i<v1.size();i++){
		r[i]=v1[i]+v2[i];
	}
	return r;
}

int main(){
	int d,n;
	cin>>d>>n;
	vector<double> r(n);
	vector<vector<double>> x(n,vector<double>(d));
	for(int i=0;i<n;i++){
		for(int j=0;j<d;j++)cin>>x[i][j];
		cin >> r[i];
	}

	vector<vector<double>> base;
	vector<double> c(x[0]);
	double radius=r[0];
	for(int i=1;i<n;i++){
		auto px=x[i]-c;
		auto pr=r[i]*r[i];
		for(const auto& v:base)
			pr-=max(0.0,pow(v*px,2)),
			px=px-v*px*v;
		pr=max(0.0,sqrt(pr));
		if(length(px)<eps)continue;
		auto w=length(px);
		base.push_back(px/w);
		auto nx=(radius*radius-pr*pr+w*w)/2/w;
		c=c+nx*base.back();
		radius=max(0.0,sqrt(radius*radius-nx*nx));
	}
//	for(int i=0;i<sc.size();i++)
//		cerr<<sc[i]<<" ";
//	cerr<<endl<<sr<<endl;
	vector<double> ret(d);
	for (int i = 0; i < d; i++) ret[i] = rand()%1000000 + 1000000;
	for(const auto& v:base){
		ret=ret-v*ret*v;
	}
	auto output=[&](const vector<double> & v){
		for(int i=0;i<v.size();i++)
			cout<<v[i]<<" ";
	};
	if(length(ret)<eps)
		output(c);
	else
		output(c - radius / length(ret) * ret);
	return 0;
}