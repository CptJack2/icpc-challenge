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
	vector<double> sc(x[0]);
	double sr=r[0];
	for(int i=1;i<n;i++){
//		auto px=x[i]-c;
//		auto pr=r[i]*r[i];
//		for(const auto& v:base)
//			pr-=max(0.0,pow(v*px,2)),
//			px=px-v*px*v;
//		pr=sqrt(pr);
//
		auto cv= sc-x[i];
		for(const auto& v:base)cv=cv-v*cv*v;
		auto cd=length(sc-x[i] - cv);
		auto sr2=max(0.0, sqrt(r[i] * r[i] - cd * cd));
		auto dist=length(cv);

		if(dist < eps)continue;
		cv=cv/dist;
		base.push_back(cv);
		auto nx= (sr * sr - sr2 * sr2 + dist * dist) /dist/ 2 ;
		sc= sc - nx * cv;
		sr=max(0.0, sqrt(sr * sr - nx * nx));
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
		output(sc);
	else
		output(sc - sr / length(ret) * ret);
	return 0;
}