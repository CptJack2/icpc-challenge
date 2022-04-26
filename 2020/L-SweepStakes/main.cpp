#include "bits/stdc++.h"
using namespace std;

constexpr double epsilon=1e-12;
vector<double> probRow,probCol;
int t;

struct ProbabilityCalc{
	vector<double> prob;//指定的格子集合中有0-i个地雷的概率
	int base,//prob下标与实际地雷数的偏移(因为前面的概率太小直接忽略)
	ignoredLB=0,ignoredUB;//prob中概率大于ε的上下界

	//向集合中添加cords里面的格子,更新集合中有0-i个地雷的概率
	void calcProb(set<pair<int,int>> cords){
		prob.resize(prob.size()+cords.size());
		for(auto pi:cords){
			double p=probRow[pi.first]+probCol[pi.second];
			//prob'[UB]<ε  ==>  prob[UB+1] = p*prob'[UB] + (1-p)*prob'[UB+1] < ε
			ignoredUB+=1;
			ignoredUB=min(t+1,ignoredUB);//最多只能有t个雷,超过就不用算了
			for (int i = ignoredUB - 1; i > ignoredLB; --i) 
				//多加一个格子,由于各格有地雷的概率独立,所以有i个mine的概率等于:前面的有i-1个mine,加上新格有1个;前面有i个,新格没有
				prob[i] = (1-p)*prob[i]+p*prob[i-1];
			prob[ignoredLB]*=(1-p);//循环里面不放if,这里由于ignoredLB前面的概率<ε,所以直接把它当作0
			//收缩概率小于ε的边界,那些地方就不计算概率了
			while(prob[ignoredLB]<epsilon)
				++ignoredLB;
			while(prob[ignoredUB-1]<epsilon)
				--ignoredUB;
		}
	}
	
	ProbabilityCalc():base(0),prob({1}),ignoredUB(1){}
	
	//查询树遇到分叉的时候更新格子的集合,重新计算概率
	ProbabilityCalc(const ProbabilityCalc& copy){
		base=copy.base+copy.ignoredLB;
		prob.resize(copy.ignoredUB-copy.ignoredLB);
		std::copy(copy.prob.begin()+copy.ignoredLB,copy.prob.begin()+copy.ignoredUB, prob.begin());
		ignoredUB=prob.size();
	}

	double operator[](int i) const{
		if(i<base+ignoredLB ||
			i>=base+ignoredUB ||
			i>t)
			return 0;
		return prob[i-base];
	}
};

struct QueryNode{
	QueryNode* left= nullptr, *right= nullptr;
	set<pair<int,int>> cords;
};

int main(){
	int m,n,q;
	cin>>m>>n>>t>>q;
	probRow.resize(m+1),
	probCol.resize(n+1);
	for (int i = 1; i <=m ; ++i)
		cin>>probRow[i];
	for (int i = 1; i <=n ; ++i)
		cin>>probCol[i];
	vector<QueryNode> queries(q+1);//最后一个是所有m*n个格子的全集,来保证查询树根上一定是格子全集
	for (int i = 0; i < q; ++i) {
		int s;
		cin>>s;
		pair<int,int> cord;
		for (int j = 0; j < s; ++j)
			cin>>cord.first>>cord.second,
			queries[i].cords.insert(cord);
	}
	//格子全集
	for (int i = 1; i <=m ; ++i)
		for (int j = 1; j <=n ; ++j)
			queries.back().cords.emplace(i,j);
	//聚合成二叉树
	function<QueryNode*(int,int)> buildTree=[&](int s,int e)->QueryNode*{
		if(s+1==e){
			return &queries[s];
		}else{
			QueryNode* ret=new QueryNode;
			ret->left=buildTree(s,(s+e)/2);
			ret->right=buildTree((s+e)/2,e);
			for(auto p:ret->left->cords)
				ret->cords.insert(p);
			for(auto p:ret->right->cords)
				ret->cords.insert(p);
			return ret;
		}
	};
	auto root=buildTree(0,q+1);
	//计算概率
	double pTMinesInField=0;
	int queryCount=0;
	function<void(QueryNode*,const ProbabilityCalc&)>
	calcProbability=[&](QueryNode* pnode,const ProbabilityCalc& pNotInSet){
		if(pnode->left== nullptr && pnode->right==nullptr){//到达查询树的叶子
			++queryCount;
			if(queryCount==q+1)
				return;//辅助用的格子全集
			ProbabilityCalc pInSet;
			pInSet.calcProb(pnode->cords);
			//整个雷场中恰好有t枚地雷的概率,第一次到树叶的时候计算
			if(pTMinesInField==0)
				for (int i = 0; i <= t; ++i)
					pTMinesInField+=pInSet[i]*pNotInSet[t-i];
			for (int i = 0; i <= pnode->cords.size(); ++i) {
				//贝叶斯公式,已知雷场共有t个雷的情况下,要查询的格子里面有i个雷的概率,等于查询格子里面有i个的概率*全集减去查询集中有t-i个的概率/整个雷场有t个的概率
				double pIMinesInQueryCords= pInSet[i] * pNotInSet[t-i] / pTMinesInField;
				cout<<setprecision(12)<<pIMinesInQueryCords<<" ";
			}
			cout<<endl;
		}else{//树中节点,向左右分支计算not in set的概率
			ProbabilityCalc pLNotInSet(pNotInSet),pRNotInSet(pNotInSet);
			set<pair<int,int>> lNotInSet,rNotInSet;
			//左递归
			for(auto pi:pnode->cords)
				if(!pnode->left->cords.count(pi))
					lNotInSet.insert(pi);
			pLNotInSet.calcProb(lNotInSet);
			calcProbability(pnode->left,pLNotInSet);
			//右递归
			for(auto pi:pnode->cords)
				if(!pnode->right->cords.count(pi))
					rNotInSet.insert(pi);
			pRNotInSet.calcProb(rNotInSet);
			calcProbability(pnode->right,pRNotInSet);
		}
	};
	calcProbability(root,ProbabilityCalc());
	return 0;
}