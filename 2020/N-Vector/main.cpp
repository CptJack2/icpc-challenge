#include "bits/stdc++.h"

using namespace std;

int main(){
	int d,n;
	cin>>d>>n;
	vector<double> e(n);
	vector<vector<double>> x(n,vector<double>(d));
	for(int i=0;i<n;i++){
		for(int j=0;j<d;j++)cin>>x[i][j];
		cin>>e[i];
	}


	return 0;
};