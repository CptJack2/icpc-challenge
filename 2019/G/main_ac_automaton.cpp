/*单纯的以Query String构建AC自动机,然后用名字去匹配,secret36测试用例会因为failure跳转次数过多tle*/
#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char ch;
	TrieNode* parent;
	TrieNode* fastLink[26];//有限的{A-Z}字符集的快速链接
	TrieNode* failure;
	bool isWord;
	set<int> queryIndexes;//这个query string是第几号, tmd还有重复的query
	int lv;//for debug
	int nodeIndex;//for debug
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
	//root for the whole trie
	TrieNode* trieRoot=new TrieNode();
	memset(trieRoot->fastLink,0,26*sizeof(TrieNode*));
	trieRoot->lv=0;
	trieRoot->nodeIndex=0;
	int totalNodeCount=1;
	vector<int> wordCount(k,0);
	//read in query trie
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
				p=pNewNode;
				pNewNode->lv=p->lv+1;
				pNewNode->nodeIndex=totalNodeCount++;
			}
			if(j==0)
				p->isWord= true,
				p->queryIndexes.insert(i);
		}
	}
	//build ac automaton failure link
	//detail algorithm: https://www.toptal.com/algorithms/aho-corasick-algorithm
	queue<TrieNode*> bfsQue;
	//trieRoot->failure=trieRoot; //root doesn't need failure transition
	for (int i = 0; i < 26; ++i)
		if(trieRoot->fastLink[i])
			trieRoot->fastLink[i]->failure=trieRoot,
			bfsQue.push(trieRoot->fastLink[i]);
		else
			trieRoot->fastLink[i]=trieRoot;
	//处理p每个孩子的failure和fastlink
	while (!bfsQue.empty()) {
		auto p = bfsQue.front();
		bfsQue.pop();
		for (int i = 0; i < 26; ++i) {
			if (p->fastLink[i] != nullptr) {
				p->fastLink[i]->failure = p->failure->fastLink[i];
				//failure指向的是它的最长后缀，将它的queryIndex缓存下来，避免遇到一个query string是另一个的后缀的情况需要沿着failure不断回溯
				for (auto ind:p->fastLink[i]->failure->queryIndexes)
					p->fastLink[i]->queryIndexes.insert(ind);
				bfsQue.push(p->fastLink[i]);
			} else
				p->fastLink[i] = p->failure->fastLink[i];
		}
	}
	//use dfs iterate over the queen tree to get answer
	stack<QueenNode*> iterStack;
	stack<TrieNode*> stateStack;
	iterStack.push(&queens[1]);
	stateStack.push(nullptr);//dummy to avoid overflow
	auto state=trieRoot;
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
		state=state->fastLink[pQueen->N-'A'];
		if(state->isWord)
			for(auto i:state->queryIndexes)
				++wordCount[i];
		stateStack.push(state);
	}
	for(auto i:wordCount)
		cout<<i<<endl;
}