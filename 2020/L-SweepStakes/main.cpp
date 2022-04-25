#include "bits/stdc++.h"
using namespace std;

struct ProbabilityCalc{
	vector<double> prob={1};
};

struct QueryNode{
	QueryNode* left= nullptr, *right= nullptr;
	set<pair<int,int>> cords;
};

int main(){
	constexpr double epsilon=1e-12;
	int m,n,t,q;
	cin>>m>>n>>t>>q;
	vector<double> probRow(m),probCol(n);
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