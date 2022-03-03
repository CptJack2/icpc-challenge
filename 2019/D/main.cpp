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
	//auto updateCounter=[&]{};
	for (int i = 0; i < n; ++i) {
		cin>>dna[i].marker>>dna[i].type;
		if(!typeCounter.count(dna[i].type))
			typeCounter[dna[i].type]=Counter{0,0};
		if(dna[i].marker=='s'){
			++typeCounter[dna[i].type].matched;
		}else{
			if(typeCounter[dna[i].type].matched!=0)
				--typeCounter[dna[i].type].matched;
			else
				++typeCounter[dna[i].type].mismatched_e;
		}
	}
	auto countAvailableTypes=[&]()->int{
		int availableTypes=0;
		for(auto d:typeCounter) {
			if (d.second == Counter{0, 0})
				++availableTypes;
		}
		return availableTypes;
	};
	int minCutPoint=n-1,minTypes=countAvailableTypes();
	for (int i = n-1; i >0 ; --i) {
		auto& counter=typeCounter[dna[i].type];
		if(dna[i].marker=='s'){
			if(counter.mismatched_e!=0)
				--counter.mismatched_e;
		} else{
			++counter.mismatched_e;
			++counter.matched;
		}
		int types=countAvailableTypes();
		if(minTypes>=types){
			minTypes=types;
			minCutPoint=i+1;
		}
	}
	cout<<minCutPoint+1<<" "<<minTypes;
}