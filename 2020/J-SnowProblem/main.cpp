#include "bits/stdc++.h"
using namespace std;

typedef pair<int,int> pii;
vector<vector<pii>> g;

struct SubTreeResult{
	int longestFromRoot;//整个子树中，从根出发，最长的路径
	int longestOne;//整个子树中,最长的一条路径
	int longestTwoFromRoot;//整个子树中,最长的两条路径,其中一条从根出发
	int longestTwo;//整个子树中,最长的两条路径的长度的和
};

void inline updateMax(int& num,initializer_list<int> candis){
	for(auto i:candis)
		num=max(num,i);
}

SubTreeResult dfs(int cur,int parent){
	int longestFromRoot=0,secondLongestFromRoot=0,//从cur出发，伸入各子树，第一和第二长的
		subLongestOne=0,//在"各子树中最长的一条"之中最长的
		subLongestTwo=0,//在"各子树中最长的两条"之中最长的
		potentialLongestTwoFromRoot=0,//"其中一条从根出发,最长的两条"的候选
		potentialLongestTwo=0;//"最长的两条"的候选
	for(auto [ch,d]:g[cur]){
		if(ch==parent)continue;
		auto subRes=dfs(ch,cur);
		//更新当前树"最长的两条"
		updateMax(potentialLongestTwo,{
			potentialLongestTwoFromRoot+subRes.longestFromRoot+d,//前面的子树中最长的"其中一条从根出发,最长的两条"+根伸入当前子树的最长路径
			subLongestTwo + subRes.longestTwo,//前面的子树中"最长的两条"+当前子树最长的两条
			longestFromRoot + subRes.longestTwoFromRoot+d,//从根伸入前面的子树中最长的路径,接入当前子树往下延申到最深,成为一条路径.另外当前子树也提供了另一条路径
			subRes.longestTwo//各个子树中最长的两条
		});
		//更新当前树"其中一条从根出发最长的两条路径"
		updateMax(potentialLongestTwoFromRoot, {
		longestFromRoot + subRes.longestOne,		//从根伸入前面一堆子树中最长的路径+当前子树中最长的路径
			subLongestOne + subRes.longestFromRoot + d	//前面一堆子树中最长的路径+从根伸入当前子树最长
		});
		//更新从根出发第一和第二长的路径
		if(subRes.longestFromRoot+d > longestFromRoot)
			secondLongestFromRoot=longestFromRoot,
			longestFromRoot= subRes.longestFromRoot + d;
		else if(subRes.longestFromRoot+d > secondLongestFromRoot)
			secondLongestFromRoot= subRes.longestFromRoot + d;
		//更新各子树中最长路径,因为上面用到的以下变量,需要表示的取最大范围是"前面所有的子树"(以避免有重复的路径),所以在后面更新
		updateMax(subLongestOne, {subRes.longestOne});//,longestFromRoot+subRes.longestFromRoot+d
		updateMax(subLongestTwo, {subRes.longestTwo});

	}
	SubTreeResult ret{0,0,0,0};
	ret.longestFromRoot=longestFromRoot;
	//树中最长路径等于：各子树中最长路径、从根出发第一第二长的路径的和，之中最大的
	updateMax(ret.longestOne,{
		subLongestOne,
		longestFromRoot + secondLongestFromRoot
	});
	ret.longestTwoFromRoot=potentialLongestTwoFromRoot;
	ret.longestTwo=potentialLongestTwo;
	return ret;
}

int main(){
	int n;
	cin>>n;
	g.resize(n+1);
	int totalLength=0;
	for (int i = 0; i < n-1; ++i) {
		int a,b,d;
		cin>>a>>b>>d;
		g[a].emplace_back(b,d);
		totalLength+= 2 * d;
	}
	auto res=dfs(1,0);
	cout<<totalLength-res.longestTwo;
	return 0;
}