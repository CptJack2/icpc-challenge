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
	int longestFromRoot=0,//从cur出发，伸入各子树，第一和第二长的
		longestOne=0,//当前树中最长的
		LongestTwoFromRoot=0,//"其中一条从根出发,最长的两条"的候选
		LongestTwo=0;//"最长的两条"的候选
	for(auto [ch,d]:g[cur]){
		if(ch==parent)continue;
		auto subRes=dfs(ch,cur);
		//因为更新用到的变量,需要表示的取最大范围是"前面所有的子树"(以避免有重复的路径),所以在各变量更新顺序如下
		//更新当前树"最长的两条"
		updateMax(LongestTwo,{
			LongestTwoFromRoot+subRes.longestFromRoot+d,	//前面的子树中最长的"其中一条从根出发,最长的两条"+根伸入当前子树的最长路径
			longestOne + subRes.longestOne,						 	//前面的子树中"最长的一条条"+当前子树最长的一条条
			longestFromRoot + subRes.longestTwoFromRoot+d,			//从根伸入前面的子树中最长的路径,接入当前子树往下延申到最深,成为一条路径.另外当前子树也提供了另一条路径
			subRes.longestTwo										//各个子树中最长的两条
		});
		//更新当前树"其中一条从根出发最长的两条路径"
		updateMax(LongestTwoFromRoot, {
			longestOne + subRes.longestFromRoot+d,	//前面子树中最长的一条+从根伸入当前子树中的最长
			longestFromRoot + subRes.longestOne,	//从根伸入前面一堆子树中最长的路径+当前子树中最长的路径
			subRes.longestTwoFromRoot+d				//各子树中最长的"其中一条从根出发,最长的两条"
		});
		//更新当前树最长
		updateMax(longestOne,{
			longestFromRoot+subRes.longestFromRoot+d,
			subRes.longestOne
		});
		//更新从根出发最长的路径
		updateMax(longestFromRoot,{subRes.longestFromRoot + d});
	}
	//cout<<"dp "<<cur<<" 0 "<<longestFromRoot<< " "<<longestOne<< " "<<LongestTwoFromRoot<< " "<<LongestTwo<<endl;
	return {longestFromRoot, longestOne, LongestTwoFromRoot, LongestTwo};
}

int main(){
	int n;
	cin>>n;
	g.resize(n+1);
	int totalLength=0;
	for (int i = 1; i <= n-1; ++i) {
		int a,b,d;
		cin>>a>>b>>d;
		g[a].emplace_back(b,d);
		g[b].emplace_back(a,d);
		totalLength+= 2 * d;
	}
	auto res=dfs(1,0);
	cout<<totalLength-res.longestTwo;
	return 0;
}