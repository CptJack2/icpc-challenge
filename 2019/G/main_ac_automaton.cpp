#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char ch;
	TrieNode* parent;
	vector<TrieNode*> children;
	TrieNode* failure;
	bool isWord;
	int index;//这个query string是第几号
};

struct QueenNode{
	char N;
	vector<QueenNode*> children;
};

int n,k;
int main(){
	cin>>n>>k;
	//read in queen tree
	vector<QueenNode> queens(n+1);
	for (int i = 1; i <= n; ++i) {
		int parentIndex;
		cin >> queens[i].N >> parentIndex;
		if(parentIndex != 0)
			queens[parentIndex].children.push_back(&queens[i]);
	}
	//read in query trie
	TrieNode* trieRoot=new TrieNode();
	trieRoot->failure=trieRoot;
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
				TrieNode* pNewNode=new TrieNode{queryStr[j], p ,{}, nullptr, false};
				p->children.push_back(pNewNode);
				p=pNewNode;
			}
			if(j==0){
				p->isWord= true;
				p->index=i;
			}
		}
	}
	//build ac automaton failure link
	/*Fail指针的求法：
	* Fail指针用BFS来求得，对于直接与根节点相连的节点来说，如果这些节点失配，他们的Fail指针直接指向root即可，其他节点其Fail指针求法如下：
	* 假设当前节点为father，其孩子节点记为child。求child的Fail指针时，首先我们要找到其father的Fail指针所指向的节点,假如是t的话，
 	* 我们就要看t的孩子中有没有和child节点所表示的字母相同的节点，如果有的话，这个节点就是child的fail指针，如果发现没有，
 	* 则需要找father->fail->fail这个节点，然后重复上面过程，如果一直找都找不到，则child的Fail指针就要指向root。*/
	queue<TrieNode*> bfsQue,bfsQue2;
	//直接与root相连的点failure设置为root
	for(auto pch:trieRoot->children) {
		pch->failure = trieRoot;
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
				for(auto pch:pf->children)
					if(pch->ch==p->ch){
						p->failure=pch;
						foundFailure= true;
						break;
					}
				if(!foundFailure)
					pf=pf->failure;
				else
					break;
				if(pf==trieRoot) {
					p->failure=trieRoot;
					break;
				}
			}
		}
		queue<TrieNode*> emptyQue;
		bfsQue.swap(emptyQue);
		bfsQue.swap(bfsQue2);
	}
	//use dfs iterate over the queen tree to get answer
	stack<QueenNode*> iterStack;
	stack<TrieNode*> stateStack;
	iterStack.push(&queens[1]);
	auto state=trieRoot;
	while(!iterStack.empty()){
		auto pQueen=iterStack.top();
		iterStack.pop();
		//the child is a leaf, go back to its parent's state
		if(pQueen== nullptr){
			state=stateStack.top();
			stateStack.pop();
			continue;
		}
		for(auto pch:pQueen->children)
			iterStack.push(pch);
		bool foundChar= false;
		while(1){
			for(auto child:state->children)
				if(child->ch==pQueen->N){
					state=child;
					foundChar= true;
					if(state->isWord)
						++wordCount[state->index];
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
		//keep track of the route's state
		stateStack.push(state);
		//meet leaf, push a null pointer to indicate it
		if(pQueen->children.empty())
			iterStack.push(nullptr);
	}
	for(auto i:wordCount)
		cout<<i<<endl;
}