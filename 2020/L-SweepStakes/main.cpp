#include "bits/stdc++.h"
using namespace std;

constexpr double epsilon=1e-12;
vector<double> probRow,probCol;

struct ProbabilityCalc{
	vector<double> prob;//指定的格子集合中有0-i个地雷的概率
	int base,//prob下标与实际地雷数的偏移(因为前面的概率太小直接忽略)
	ignoredLB=0,ignoredUB;//prob中概率大于ε的上下界

	//向集合中添加cords里面的格子,更新集合中有0-i个地雷的概率
	void calcProb(set<pair<int,int>> cords){
		prob.resize(prob.size()+cords.size());
		for(auto it=cords.begin();it!=cords.end();++it){
			double p=probRow[it->first]+probCol[it->second];
			//prob'[UB]<ε  ==>  prob[UB+1] = p*prob'[UB] + (1-p)*prob'[UB+1] < ε
			ignoredUB+=1;
			for (int i = ignoredUB - 1; i > ignoredLB; --i) 
				//多加一个格子,由于各格有mine概率独立,所以有i个mine的概率等于:前面的有i-1个mine,加上新格有1个;前面有i个,新格没有
				prob[i] = (1-p)*prob[i]+p*prob[i-1];
			prob[ignoredLB]*=(1-p);//循环里面不放if,这里由于ignoredLB前面的概率<ε,所以直接把它当作0
			//收缩概率小于ε的边界,那些地方就不计算概率了
			while(prob[ignoredLB]<epsilon)
				++ignoredLB;
			while(prob[ignoredUB]<epsilon)
				--ignoredUB;
		}
	}
	
	ProbabilityCalc():base(0),prob({1}),ignoredUB(1){}
	
	//查询数遇到分叉的时候更新格子的集合,重新计算概率
	ProbabilityCalc(const ProbabilityCalc& copy){
		base=copy.base+copy.ignoredLB;
		prob.resize(copy.ignoredUB-copy.ignoredLB);
		std::copy(copy.prob.begin()+copy.ignoredLB,copy.prob.begin()+copy.ignoredUB, prob.begin());
		ignoredUB=prob.size();
	}
};

struct QueryNode{
	QueryNode* left= nullptr, *right= nullptr;
	set<pair<int,int>> cords;
};

int main(){
	int m,n,t,q;
	cin>>m>>n>>t>>q;
	probRow.resize(m),
	probCol.resize(n);
	for (int i = 0; i <m ; ++i) 
		cin>>probRow[i];
	for (int i = 0; i <n ; ++i)
		cin>>probCol[i];
	vector<QueryNode> queries(q+1);
	for (int i = 0; i < q; ++i) {
		int s;
		cin>>s;
		pair<int,int> cord;
		for (int j = 0; j < s; ++j)
			cin>>cord.first>>cord.second,
			queries[i].cords.insert(cord),
			queries.back().cords.insert(cord);
	}
	//聚合成二叉树
	function<QueryNode*(int,int)> buildTree=[&](int s,int e)->QueryNode*{
		QueryNode* ret=new QueryNode;
		if(s+1==e){
			return &queries[s];
		}else{
			ret->left=buildTree(s,(s+e)/2);
			ret->right=buildTree((s+e)/2,e);
			for(auto p:ret->left->cords)
				ret->cords.insert(p);
			for(auto p:ret->right->cords)
				ret->cords.insert(p);
		}
		return ret;
	};
	auto root=buildTree(0,q+1);
	//计算概率
	double pTMinesInField=0;
	function<void(int,int)> calcProbability=[&](int s,int e){
		if(s+1==e){

		}else{

		}
	};
	return 0;
}