#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char c;
	int index;
	TrieNode* parent;
	vector<TrieNode*> children;
	vector<TrieNode*> binayAncestors;//倍增的祖先
};

int n,k;
int main(){
	cin>>n>>k;
	vector<TrieNode> nodes(n+1);
	map<char,int> usedAlpha;
	for (int i = 1; i <= n; ++i) {
		char c;
		int parentIndex;
		cin >> c >> parentIndex;
		nodes[i].c=c;
		nodes[i].index=i;
		nodes[i].parent=&nodes[parentIndex];
		nodes[i].parent->children.push_back(&nodes[i]);
		usedAlpha[c]=0;
	}
	int alphaRank=1;
	for(auto& p:usedAlpha)
		p.second=alphaRank++;
	//寻找倍增祖先
	vector<TrieNode*> inheritanceStack;
	int trieHeight=0;//字典树高度
	vector<TrieNode*> iterationStack;
	iterationStack.push_back(&nodes[1]);
	while(!iterationStack.empty()){
		auto node =  iterationStack.back();
		if(node== nullptr){
			iterationStack.pop_back();//pop nullptr indicator
			iterationStack.pop_back();//pop the parent that finished its procedure
			inheritanceStack.pop_back();
			continue;
		}
		//binary lifting to find ancestors
		for(int lv=1; lv <= inheritanceStack.size(); lv*=2)
			node->binayAncestors.push_back(inheritanceStack[inheritanceStack.size() - lv]);
		inheritanceStack.push_back(node);
		if(inheritanceStack.size() > trieHeight)
			trieHeight=inheritanceStack.size();
		//use a nullptr to indicate that all children are iterated
		iterationStack.push_back(nullptr);
		for(auto ch:node->children)
			iterationStack.push_back(ch);
	}
	//统计第一个字母的排序
	vector<int> rank(n+1);//queen name的前2^k个字母的排序, rank[node index]=字典序排名
	//根据字母序为rank赋初始值
	for (int i = 1; i <= n; ++i)
		rank[i]=usedAlpha[nodes[i].c];
	struct rankKey{
		int rankFront;//queen name的前缀前半段rank
		int rankBack;//queen name的前缀后半段rank
		int index;
	};
	for(int power=0;int dist=pow(2,power)<=trieHeight;power++){
		auto cmp=[&](const rankKey& p1,const rankKey& p2){
			return p1.rankFront<p2.rankFront || p1.rankFront==p2.rankFront && p1.rankBack<p2.rankBack;
		};
		vector<rankKey> sortRank(n+1);
		for(int i=1;i<=n;++i)
			sortRank[i]=rankKey{
					rank[i],
					power < nodes[i].binayAncestors.size() ?
					rank[ nodes[i].binayAncestors[power]->index ] :
					0,
					i
			};
		sort(sortRank.begin()+1,sortRank.end(),cmp);
		int keyRank=1;
		//更新rank
		vector<int> newRank(n+1);
		for (int i = 1; i <= n; ) {
			do {
				newRank[sortRank[i].index] = keyRank;
				++i;
			}while(!(i > n ||
					 sortRank[i - 1].rankFront != sortRank[i].rankFront || sortRank[i - 1].rankBack != sortRank[i].rankBack));
			++keyRank;
		}
		rank=newRank;
	}
	//转换rank
	vector<int> prefixArray(n+1);//prefixArray[字典序排名]=node index
	for (int i = 1; i <=n ; ++i)
		prefixArray[rank[i]]=i;
	//读入查询串,二分查找
	for (int i = 0; i < k; ++i) {
		string queryStr;
		cin >> queryStr;
		//判断node[index]的string是否小于query string, lb指示是lower_bound调用或是upper_bound调用
		auto cmp=[&](int index,const string& qstr,bool lb)->bool{
			auto p=&nodes[index];
			for(int i=0;i<qstr.size();++i){
				char c=qstr[i];
				// queen name < qstr
				if(p->c < c)
					return lb;
					//queen name > qstr
				else if(p->c > c)
					return !lb;
				else
				if(p->parent)
					p=p->parent;
				else
					//exactly same string
				if(i==qstr.size()-1)
					return false;
				else
					//qstr is longer, should go behind queen name
					return lb;
			}
			/*qstr is shorter, if lb, should go before queen name;
			if ub, should go behind queen name*/
			return false;
		};
		auto lb= lower_bound(prefixArray.begin()+1,prefixArray.end(),queryStr,
							 [&](int index,const string& qstr){
								 bool t=cmp(index, qstr, true);
								 return t;
							 });
		auto ub= upper_bound(prefixArray.begin()+1,prefixArray.end(),queryStr,
							 [&](const string& qstr,int index){
								 bool t=cmp(index, qstr, false);
								 return t;
							 });
		int count=ub-lb;
		cout<<count<<endl;
	}
}