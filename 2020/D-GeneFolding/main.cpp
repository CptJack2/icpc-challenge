/*经过一些修改的Manacher算法,用贪心的思路不断缩短两头的回文串*/
#include "bits/stdc++.h"
using namespace std;

int main(){
	string gene;
	cin>>gene;
	string mGene;
	mGene.resize(2*gene.size()+1);
	int i=0;
	for(auto c:gene)
		mGene[2*i]='#',
		mGene[2*i+1]=c,
		++i;
	mGene.back()='#';
	vector<int>radius(mGene.size(),0);
	int head=0, tail=mGene.size()-1;
	int rBorder=-1, center=-1;
	for(int i=0;i<mGene.size();++i){
		radius[i]= rBorder >= i ? min(radius[center + center - i], rBorder - i) : 1;
		while(i+radius[i]<mGene.size() && i-radius[i]>=0 && mGene[i+radius[i]]==mGene[i-radius[i]])
			++radius[i];
		if(i+radius[i]-1>rBorder)
			rBorder=i+radius[i]-1,
			center=i;
		if(i%2==0 && radius[i]-1>=i-head)//char is a '#' and it can reach head
			head=i;
	}
	for(int i=mGene.size()-1;i>=head;i-=2){
		if(radius[i]-1>=tail-i)
			tail=i;
	}
	int ans=(tail-head)/2;
	cout<<ans;
}