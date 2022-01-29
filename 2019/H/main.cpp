#include"bits/stdc++.h"

using namespace std;

int main(){
	int n,k;
	cin>>n>>k;
	//i点的出站
	vector<int> to(n+1);
	//i点的入站
	vector<vector<int>>from(n+1);
	vector<int> ans(n+1,1);
	for(int i=1;i<=n;++i){
		cin>>to[i];
		from[to[i]].push_back(i);
	}
	set<int> visited;
	//每个点遍历
	for(int i=1;i<=n;++i){
		if(visited.find(i)!=visited.end())
			continue;
		int circle_start=i;
		//找圈
		while(visited.find(to[circle_start])!=visited.end()){
			visited.insert(circle_start);
			circle_start=to[circle_start];
		}
		circle_start=to[circle_start];
		//将圈中所有点放到circle
		vector<int>circle;
		int p_circle=circle_start;
		do {
			circle.push_back(p_circle);
			p_circle=to[p_circle];
		}while(p_circle!=circle_start);
		//从圈上找分叉
		for(auto circ_it=circle.begin(); circ_it != circle.end(); ++circ_it){
			//找圈上的上一个
			auto circle_prev= prev(circ_it);
			if(circ_it == circle.begin())
				circle_prev=prev(circle.end());
			//否则就是一个树枝分叉
			vector<vector<int>> tree_lv;
			tree_lv.push_back(vector<int>{*circ_it});
			//以圈上点为root，层高为0，广度优先遍历记录每一层的孩子数
			while(!tree_lv.back().empty()){
				auto& dfs_que=tree_lv.back();
				tree_lv.emplace_back();
				for(auto parent=dfs_que.begin();parent!=dfs_que.end();++parent){
					for(auto ch:from[*parent]){
						//对于根节点要跳过它圈上的上一个
						if(*parent==*circ_it && ch == *circle_prev)
							continue;
						tree_lv.back().push_back(ch);
					}
				}
			}

		}
	}
}

