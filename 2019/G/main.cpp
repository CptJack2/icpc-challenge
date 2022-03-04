#include "bits/stdc++.h"
using namespace std;

struct TrieNode{
	char c;
	TrieNode* parent;
};

int n,k;
int main(){
	cin>>n>>k;
	set<char> usedAlpha;
	vector<TrieNode> nodes(n+1);
	for (int i = 1; i <= n; ++i) {
		char c;
		int index;
		cin>>c>>index;
		nodes[i].c=c;
		nodes[i].parent=&nodes[index];
		usedAlpha.insert(c);
	}
	vector<int> alphaRanking(26);
	int count=0;
	for(char c:usedAlpha)
		alphaRanking[c-'A']=count++;
	vector<int> sortedNodes(n);
	for (int i = 0; i < n; ++i)
		sortedNodes[i]=i+1;
	auto cmp=[&](const int& node1, const int& node2){
		TrieNode* p1=&nodes[node1], *p2=&nodes[node2];
		while(p1!= nullptr && p2!= nullptr){
			if(p1->c < p2->c)
				return true;
			else if(p1->c > p2->c)
				return false;
			else{
				p1=p1->parent;
				p2=p2->parent;
			}
		}
		//exactly two same name
		return true;
	};
	sort(sortedNodes.begin(),sortedNodes.end(),cmp);
	lower_bound(sortedNodes.begin(),sortedNodes.end(),)
}