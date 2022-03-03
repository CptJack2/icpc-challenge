#include "bits/stdc++.h"
using namespace std;

int n;
struct Gene{
	char marker;
	int type;
};
struct Counter{
	int mismatched_e;//不匹配的e
	int matched;//s的数量减去匹配的e的数量
	bool operator ==(const Counter& b){return mismatched_e==b.mismatched_e && matched==b.matched;}
};
int main(){
	cin>>n;
	vector<Gene> dna(n);
	map<int,Counter> typeCounter;//gene type -> counter
	//读入数据,从1点前切开
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
	//当前切点可行的type数量
	int availableTypes=0;
	for(auto d:typeCounter) {
		if (d.second == Counter{0, 0})
			++availableTypes;
	}
	int minCutPoint=0,maxTypes=availableTypes;
	//从逆时针方向调整切点
	for (int i = n-1; i >0 ; --i) {
		auto& counter=typeCounter[dna[i].type];
		//如果s和e数量不相等,这个type不可能nested了,直接不管它了
		if(counter.matched- counter.mismatched_e!=0){
			//虽然这个切点不影响availableTypes,但题目要求切点可以尽量前移
			if(maxTypes==availableTypes && minCutPoint>i)
				minCutPoint=i;
			continue;
		}
		//调整可行type数量
		if(counter==Counter{0, 0})--availableTypes;
		//相当于将当前的gene移到序列头
		if(dna[i].marker=='s'){
			if (counter.mismatched_e != 0) {
				--counter.mismatched_e;
				--counter.matched;
			}
		} else{
			++counter.mismatched_e;
			++counter.matched;
		}
		//调整可行type数量
		if(counter==Counter{0, 0})++availableTypes;
		//更新最佳切法
		if(maxTypes < availableTypes || maxTypes==availableTypes && minCutPoint>i ){
			maxTypes=availableTypes;
			minCutPoint=i;
		}
	}
	//代码里第一个下标是0,调整一下
	cout <<minCutPoint+1 << " " << maxTypes;
}