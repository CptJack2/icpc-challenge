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
	for (int i = 1; i <= n; ++i) {
		char c;
		int index;
		cin>>c>>index;
		nodes[i].c=c;
		nodes[i].index=i;
		nodes[i].parent=&nodes[index];
		nodes[i].parent->children.push_back(&nodes[i]);
	}
	//寻找倍增祖先
	vector<TrieNode*> dfsStack;
	int trieHeight=0;//字典树高度
	function<void(TrieNode*)> dfs=[&](TrieNode* node){
		for(int lv=1;lv<dfsStack.size();lv*=2)
			node->binayAncestors.push_back(dfsStack[dfsStack.size()-lv]);
		dfsStack.push_back(node);
		if(dfsStack.size() > trieHeight)
			trieHeight=dfsStack.size();
		for(auto ch:node->children)
			dfs(ch);
		dfsStack.pop_back();
	};
	dfs(&nodes[1]);
	//统计第一个字母的排序
	vector<int> rank(n+1);//queen name的前2^k个字母的排序, rank[node index]=字典序排名
	for (int i = 0; i <= n; ++i)
		rank[i]=nodes[i].c-'A'+1;
	struct rankKey{
		int rankFront;//queen name的前缀前半段rank
		int rankBack;//queen name的前缀后半段rank
	};
	for(int power=0;int dist=pow(2,power)<=trieHeight;power++){
		auto cmp=[&](const rankKey& p1,const rankKey& p2){
			return p1.rankFront<p2.rankFront || p1.rankFront==p2.rankFront && p1.rankBack<p2.rankBack;
		};
		map<rankKey,int,decltype(cmp)>distinctRankKeys(cmp);//用map对不同的rankKey排序
		for(int i=1;i<=n;++i)
			distinctRankKeys[rankKey{rank[i], i+dist<n? rank[i+dist] : 0}]=distinctRankKeys.size()+1;
		//更新rank
		for (int i = 1; i <= n; ++i)
			rank[i]=distinctRankKeys[rankKey{rank[i], i+dist<n? rank[i+dist] : 0}];
	}
	//转换rank
	vector<int> prefixArray(n+1);//prefixArray[字典序排名]=node index
	for (int i = 1; i <=n ; ++i)
		prefixArray[rank[i]]=i;
	//读入查询串,二分查找
	for (int i = 0; i < k; ++i) {
		string queryStr;
		cin >> queryStr;

		auto cmp=[&](int index,const string& qstr,bool lb)->bool{
			auto p=&nodes[index];
			for(auto c:qstr){
				if(p->c < c)
					return lb;
				else if(p->c > c)
					return !lb;
				else
					if(p->parent)
						p=p->parent;
					else
						//exactly same string
						if(c==qstr.back())
							return false;
						else
							//qstr is longer, and prefixes are equal
							return !lb;
			}
			//qstr is shorter, and prefixes are equal
			return lb;
		};
		auto lb= lower_bound(prefixArray.begin(),prefixArray.end(),queryStr,
	   		[&](int index,const string& qstr){
				return cmp(index,qstr,true);
			});
		auto ub= upper_bound(prefixArray.begin(),prefixArray.end(),queryStr,
	   		[&](const string& qstr,int index){
				return cmp(index,qstr, false);
			});
		cout<<ub-lb<<endl;
	}
}