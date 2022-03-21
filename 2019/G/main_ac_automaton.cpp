#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char ch;
	TrieNode* parent;
	vector<TrieNode*> children;
	TrieNode* failure;
	TrieNode* failureInDict;
	bool isWord;
	vector<int> indexes;//这个query string是第几号, tmd还有重复的query
	int lv;//for debug
};

struct QueenNode{
	char N;
	vector<QueenNode*> children;
	QueenNode* parent;//for debug
	int lv;//for debug
	int index;//for debug
};

int n,k;
int main(){
	cin>>n>>k;
	//read in queen tree
	vector<QueenNode> queens(n+1);
	for (int i = 1; i <= n; ++i) {
		int parentIndex;
		cin >> queens[i].N >> parentIndex;
		if(parentIndex != 0){
			queens[parentIndex].children.push_back(&queens[i]);
			queens[i].parent=&queens[parentIndex];
			queens[i].lv=queens[parentIndex].lv+1;
		} else
			queens[i].lv=1;
		queens[i].index=i;
	}
	//read in query trie
	TrieNode* trieRoot=new TrieNode();
	trieRoot->failure=trieRoot;
	trieRoot->failureInDict=trieRoot;
	trieRoot->lv=0;
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
				TrieNode* pNewNode=new TrieNode{queryStr[j], p ,{}, nullptr, nullptr,false,{},p->lv+1};
				p->children.push_back(pNewNode);
				p=pNewNode;
			}
			if(j==0){
				p->isWord= true;
				p->indexes.push_back(i);
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
	queue<TrieNode*> bfsQue,bfsQue2;
	//直接与root相连的点failure设置为root
	for(auto pch:trieRoot->children) {
		pch->failure = trieRoot;
		pch->failureInDict = pch;
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
						break;
					}
				//if not, keep following its failure
				if(!foundFailure && pf!=trieRoot)
					pf=pf->failure;
				//can't follow more
				else if(!foundFailure && pf==trieRoot) {
					p->failure = trieRoot;
					p->failureInDict = trieRoot;
					break;
				//found failure link, just quit
				} else
					break;
			}
			//follow failure links to find first word( or root) in dict
			if(p->isWord)
				p->failureInDict=p;
			else
				p->failureInDict=p->failure->failureInDict;
		}
		queue<TrieNode*> emptyQue;
		bfsQue.swap(emptyQue);
		bfsQue.swap(bfsQue2);
	}
	//use dfs iterate over the queen tree to get answer
	stack<QueenNode*> iterStack;
	stack<TrieNode*> stateStack;
	iterStack.push(&queens[1]);
	stateStack.push(nullptr);//dummy to avoid overflow
	auto state=trieRoot;
	//state compute cache
	map<pair<TrieNode*,char>,TrieNode*> runCache;
	while(!iterStack.empty()){
		auto pQueen=iterStack.top();
		iterStack.pop();
		//the child is a leaf, go back to its parent's state
		if(pQueen== nullptr){
			stateStack.pop();
			state=stateStack.top();
			continue;
		}
		iterStack.push(nullptr);
		for(auto pch:pQueen->children)
			iterStack.push(pch);
		//check if result cached
		if(runCache.count(make_pair(state,pQueen->N)))
			state=runCache[make_pair(state,pQueen->N)];
		else{//run state machine
			//modify current state according to input
			bool foundChar= false;
			TrieNode* origState=state;
			while(1){
				for(auto child:state->children)
					if(child->ch==pQueen->N){
						state=child;
						foundChar= true;
						break;
					}
				//following failure link back to root, and still don't have the char, just let it go
				if(state==trieRoot)
					break;
				if(!foundChar)
					state=state->failure;
				else
					break;
			}
			runCache[make_pair(origState,pQueen->N)]=state;
		}
		//update word count using suffix link and end word link
		auto pTraceBack=state;
		while(1){
			pTraceBack=pTraceBack->failureInDict;
			if(pTraceBack==trieRoot)
				break;
			for (auto ind:pTraceBack->indexes) {
				++wordCount[ind];
			}
			pTraceBack=pTraceBack->failure;
		}
		//keep track of the route's state
		stateStack.push(state);
	}
	for(auto i:wordCount)
		cout<<i<<endl;
}