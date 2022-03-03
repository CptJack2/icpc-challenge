#include "bits/stdc++.h"
using namespace std;

int n;
struct Gene{
	char marker;
	int type;
};
struct Counter{
	int mismatched_e;
	int matched;
	bool operator ==(const Counter& b){return mismatched_e==b.mismatched_e && matched==b.matched;}
};
int main(){
	cin>>n;
	vector<Gene> dna(n);
	map<int,Counter> typeCounter;//gene type -> counter
	for (int i = 0; i < n; ++i) {
		cin>>dna[i].marker>>dna[i].type;
		if(!typeCounter.count(dna[i].type))
			typeCounter[dna[i].type]=Counter{0,0};
		if(dna[i].marker=='s'){
			++typeCounter[dna[i].type].matched;
		}else{
			if(typeCounter[dna[i].type].matched>0)
				--typeCounter[dna[i].type].matched;
			else
				++typeCounter[dna[i].type].mismatched_e;
		}
	}
	int availableTypes=0;
	for(auto d:typeCounter) {
		if (d.second == Counter{0, 0})
			++availableTypes;
	}
	int minCutPoint=0,maxTypes=availableTypes;
	for (int i = n-1; i >0 ; --i) {
		auto& counter=typeCounter[dna[i].type];
		//如果s和e数量不相等,这个type不可能nested了,直接不管它了
		if(counter.matched- counter.mismatched_e!=0)continue;
		if(counter==Counter{0, 0})--availableTypes;
		if(dna[i].marker=='s'){
			if (counter.mismatched_e != 0) {
				--counter.mismatched_e;
				--counter.matched;
			}
		} else{
			++counter.mismatched_e;
			++counter.matched;
		}
		if(counter==Counter{0, 0})++availableTypes;
		if(maxTypes < availableTypes || maxTypes==availableTypes && minCutPoint>i ){
			maxTypes=availableTypes;
			minCutPoint=i;
		}
	}
	cout <<minCutPoint+1 << " " << maxTypes;
}