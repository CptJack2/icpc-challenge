#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char ch;
	TrieNode* parent;
	vector<TrieNode*> children;
	TrieNode* failure;
	vector<TrieNode*> dfsChildren;
	int val;
	vector<int> queryIndexes;//这个query string是第几号, tmd还有重复的query
	int lv;//for debug
};

int n,k;
int main(){
	cin>>n>>k;
	//root for the whole trie
	TrieNode* trieRoot=new TrieNode();
	trieRoot->lv=0;
	//read in queen tree
	vector<TrieNode> queens(n+1);
	for (int i = 1; i <= n; ++i) {
		int parentIndex;
		cin >> queens[i].ch >> parentIndex;
		queens[i].val=1;
		if(parentIndex != 0){
			queens[parentIndex].children.push_back(&queens[i]);
			queens[i].parent=&queens[parentIndex];
			queens[i].lv=queens[parentIndex].lv+1;
		} else {
			trieRoot->children.push_back(&queens[i]);
			queens[i].parent=trieRoot;
			queens[i].lv = 1;
		}
	}
	//read in query trie
	vector<int> wordCount(k,0);
	for (int i = 0; i < k; ++i) {
		string queryStr;
		cin >> queryStr;
		TrieNode* p=trieRoot;
		for (int j = queryStr.size() - 1; j >= 0 ; --j) {
			bool inTrie= false;
			for(auto pchild:p->children)
				if(pchild->ch == queryStr[j]){
					p=pchild;
					inTrie= true;
					break;
				}
			if(!inTrie){
				TrieNode* pNewNode=new TrieNode;
				pNewNode->ch=queryStr[j];
				pNewNode->parent=p;
				pNewNode->lv=p->lv+1;
				p->children.push_back(pNewNode);
				p=pNewNode;
			}
			if(j==0){
				p->queryIndexes.push_back(i);
			}
		}
	}
	//build ac automaton failure link
	//detail algorithm: https://www.toptal.com/algorithms/aho-corasick-algorithm
	/*Fail指针的求法：
	* Fail指针用BFS来求得，对于直接与根节点相连的节点来说，如果这些节点失配，他们的Fail指针直接指向root即可，其他节点其Fail指针求法如下：
	* 假设当前节点为father，其孩子节点记为child。求child的Fail指针时，首先我们要找到其father的Fail指针所指向的节点,假如是t的话，
 	* 我们就要看t的孩子中有没有和child节点所表示的字母相同的节点，如果有的话，这个节点就是child的fail指针，如果发现没有，
 	* 则需要找father->fail->fail这个节点，然后重复上面过程，如果一直找都找不到，则child的Fail指针就要指向root。*/
	trieRoot->failure=trieRoot;
	queue<TrieNode*> bfsQue,bfsQue2;
	//直接与root相连的点failure设置为root
	for(auto pch:trieRoot->children) {
		pch->failure = trieRoot;
		trieRoot->dfsChildren.push_back(pch);
		for(auto pgch:pch->children)
			bfsQue.push(pgch);
	}
	while (!bfsQue.empty()){
		while (!bfsQue.empty()) {
			auto p=bfsQue.front();
			bfsQue.pop();
			for(auto pch:p->children)
				bfsQue2.push(pch);
			bool foundFailure= false;
			auto pf=p->parent->failure;
			while(1){
				//find whether its parent's failure has a child with same char
				for(auto pch:pf->children)
					if(pch->ch==p->ch){
						p->failure=pch;
						foundFailure= true;
						p->failure->dfsChildren.push_back(p);
						break;
					}
				//if not, keep following its failure
				if(!foundFailure && pf!=trieRoot)
					pf=pf->failure;
				//can't follow more
				else if(!foundFailure && pf==trieRoot) {
					p->failure = trieRoot;
					trieRoot->dfsChildren.push_back(p);
					break;
				//found failure link, just quit
				} else
					break;
			}
		}
		queue<TrieNode*> emptyQue;
		bfsQue.swap(emptyQue);
		bfsQue.swap(bfsQue2);
	}
	//use dfs iterate over the queen tree to get answer
	function<void(TrieNode*)> dfs=[&](TrieNode* p){
		for(auto pch:p->dfsChildren) {
			dfs(pch);
			p->val+=pch->val;
		}
		for(auto i:p->queryIndexes)
			wordCount[i]=p->val;
	};
	dfs(trieRoot);
	for(auto i:wordCount)
		cout<<i<<endl;
}