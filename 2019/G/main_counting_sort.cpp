/*这个版本跟main.cpp里面的,除了中间进行排序的部分是用基数排序外,其他的地方都是一样的.
 * 但是使用基数排序后,会TLE,perf.svg里是执行的火焰图,看不出瓶颈在哪.
 * 题解的理论上使用基数排序,复杂度是O(nlogn),而用std sort复杂度是O(nlog^2n),但实际上运行起来还是有差别的.*/
#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char c;
	int index;
	TrieNode* parent;
	vector<TrieNode*> children;
	vector<TrieNode*> binayAncestors;//倍增的祖先
	int rankFront;
	int rankBack;
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
	iterationStack.reserve(n);
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
	//基数排序的两个桶
	auto pushSortRank=[&](vector<vector<TrieNode*>>* pRank,int index,TrieNode* pNode){
		if((*pRank)[index].size()==0)
			(*pRank)[index].reserve(n/4);
		(*pRank)[index].push_back(pNode);
	};
	vector<vector<TrieNode*>> sortRank1(n+1),sortRank2(n+1);
	vector<vector<TrieNode*>>* pSortingRank=&sortRank1,*pSortedRank=&sortRank2;
	for (int i = 1; i <= n; ++i) {
		int r=usedAlpha[nodes[i].c];
		nodes[i].rankFront=r;
		pushSortRank(pSortedRank,r,&nodes[i]);
	}
	for(int power=0;pow(2,power)<=trieHeight;power++){
		for(auto& List:*pSortingRank)
			List.clear();
		for(auto& List:*pSortedRank)
			for(auto it=List.begin();it!=List.end();++it){
				auto p=*it;
				int currentNodeIndex=p->index;
				if(power < nodes[currentNodeIndex].binayAncestors.size()){
					int binaryAncestorIndex=nodes[currentNodeIndex].binayAncestors[power]->index;
					p->rankBack=nodes[binaryAncestorIndex].rankFront;
					pushSortRank(pSortingRank,p->rankBack,p);
				}else{
					p->rankBack=0;
					pushSortRank(pSortingRank,0,p);
				}
			}
		for(auto& List:*pSortedRank)
			List.clear();
		for(auto& List:*pSortingRank)
			for(auto p:List)
				pushSortRank(pSortedRank,p->rankFront,p);
		int newRank=1;
		pair<int,int> oldKey;
		bool firstKey=true;
		for(auto& List:*pSortedRank)
			for(auto p:List) {
				//后面的关键字先基数排序
				if(firstKey){
					oldKey = make_pair(p->rankFront, p->rankBack);
					firstKey= false;
				}else if (oldKey.first != p->rankFront || oldKey.second != p->rankBack) {
					++newRank;
					oldKey = make_pair(p->rankFront, p->rankBack);
				}
				p->rankFront = newRank;
				p->rankBack = 0;
			}
		for(auto& List:*pSortingRank)
			List.clear();
		for(auto& List:*pSortedRank)
			for(auto p:List)
				//前面的关键字基数排序
				pushSortRank(pSortingRank,p->rankFront,p);
		swap(pSortedRank,pSortingRank);
	}
	//生成prefixArray
	vector<int> prefixArray(n+1);//prefixArray[字典序排名]=node index
	for (int i = 1; i <=n ; ++i){
		auto List=(*pSortedRank)[i];
		prefixArray[i]=(*pSortedRank)[i].front()->index;
	}
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