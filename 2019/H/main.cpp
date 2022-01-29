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
	for(int node_index=1; node_index <= n; ++node_index){
		if(visited.find(node_index) != visited.end())
			continue;
		int circle_start=node_index;
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
			//树中节点，每一层的可达数加上往后k层的节点数
			for(int i=0;i<=tree_lv.size()-2;++i){
				for(int j=1;j<=k;j++){
					if(i+j>=tree_lv.size())
						break;
					for(auto ind:tree_lv[i])
						ans[ind]+=tree_lv[i+j].size();
				}
			}
			//树到圈中节点(除数根外)
			int cir_n=min(k-1,int(circle.size()-1));
			auto cir_it=circle_prev;
			for(int i=0;i<cir_n;i++){
				int lv=min(int(tree_lv.size())-1,k-1-i);
				for(int j=1;j<=lv;++j)
					ans[*cir_it]+=tree_lv[lv].size();
				if(cir_it==circle.begin())
					circ_it=prev(circle.end());
				else
					cir_it= prev(cir_it);
			}
			//圈中节点互相访问

		}
	}
}

