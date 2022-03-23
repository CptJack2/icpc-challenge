#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char ch;
	TrieNode* fastLink[26];//有限的{A-Z}字符集的快速链接
	TrieNode* failure;
	vector<TrieNode*> dfsChildren;//dfs搜索时的子节点
	int val;
	vector<int> queryIndexes;//这个query string是第几号, tmd还有重复的query
	int lv;//for debug
	int nodeIndex;//for debug
};

int n,k;
int main(){
	cin>>n>>k;
	//root for the whole trie
	TrieNode* trieRoot=new TrieNode();
	trieRoot->lv=0;
	trieRoot->nodeIndex=0;
	trieRoot->val=0;
	memset(trieRoot->fastLink,0,26*sizeof(TrieNode*));
	int totalNodeCount=1;
	//read in queen tree
	vector<TrieNode> queens(n+1);
	for (int i = 1; i <= n; ++i) {
		int parentIndex;
		cin >> queens[i].ch >> parentIndex;
		queens[i].val=1;
		auto parent=parentIndex!=0 ? &queens[parentIndex]:trieRoot;
		parent->fastLink[queens[i].ch-'A']=&queens[i];
		queens[i].nodeIndex=totalNodeCount++;
		queens[i].lv=queens[parentIndex].lv+1;
	}
	//read in query trie
	vector<int> wordCount(k,0);
	vector<TrieNode*> queryNode(k);//for debug
	for (int i = 0; i < k; ++i) {
		string queryStr;
		cin >> queryStr;
		TrieNode* p=trieRoot;
		for (int j = queryStr.size() - 1; j >= 0 ; --j) {
			if(auto pch=p->fastLink[queryStr[j]-'A'])
				p=pch;
			else{
				TrieNode* pNewNode=new TrieNode;
				memset(pNewNode->fastLink,0,26*sizeof(TrieNode*));
				pNewNode->ch=queryStr[j];
				p->fastLink[queryStr[j]-'A']=pNewNode;
				pNewNode->val=0;
				p=pNewNode;
				pNewNode->lv=p->lv+1;
				pNewNode->nodeIndex=totalNodeCount++;
			}
			if(j==0){
				p->queryIndexes.push_back(i);
				queryNode[i]=p;
			}
		}
	}
	//build ac automaton failure link
	//detail algorithm: https://www.toptal.com/algorithms/aho-corasick-algorithm
	queue<TrieNode*> bfsQue;
	//trieRoot->failure=trieRoot; //root doesn't need failure transition
	for (int i = 0; i < 26; ++i)
		if(trieRoot->fastLink[i])
			trieRoot->fastLink[i]->failure=trieRoot,
			bfsQue.push(trieRoot->fastLink[i]),
			trieRoot->dfsChildren.push_back(trieRoot->fastLink[i]);
		else
			trieRoot->fastLink[i]=trieRoot;
	//处理p每个孩子的failure和fastlink
	while (!bfsQue.empty()) {
		auto p = bfsQue.front();
		bfsQue.pop();
		for (int i = 0; i < 26; ++i) {
			if (p->fastLink[i] != nullptr)
				p->fastLink[i]->failure = p->failure->fastLink[i],
				bfsQue.push(p->fastLink[i]),
				p->fastLink[i]->failure->dfsChildren.push_back(p->fastLink[i]);
			else
				p->fastLink[i] = p->failure->fastLink[i];
		}
	}
	//use dfs iterate over the queen tree to get answer
	vector<TrieNode*> iterStack;
	iterStack.push_back(trieRoot);
	while(!iterStack.empty()){
		auto p=iterStack.back();
		if(p==nullptr){
			//pop null pointer
			iterStack.pop_back();
			p=iterStack.back();
			for(auto pch:p->dfsChildren)
				p->val+=pch->val;
			for(auto i:p->queryIndexes)
				wordCount[i]=p->val;
			//pop the parent frame
			iterStack.pop_back();
			continue;
		}
		//push pop frame indicator
		iterStack.push_back(nullptr);
		for(auto pch:p->dfsChildren)
			iterStack.push_back(pch);
	}
	for(auto i:wordCount)
		cout<<i<<endl;
}