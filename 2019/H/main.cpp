#include"bits/stdc++.h"

using namespace std;

int main(){
	int n,k;
	cin>>n>>k;
	//i点的出站
	vector<int> to(n+1);
	//i点的入站
	vector<vector<int>>from(n+1);
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
		for(auto it=circle.begin();it!=circle.end();++it){
			//找圈上的上一个
			auto circle_prev= prev(it);
			if(it==circle.begin())
				circle_prev=prev(circle.end());
			for(auto r:from[*it]){
				//如果来者是圈上上一个跳过
				if(r==*circle_prev)
					continue;
				//否则就是一个树枝分叉

			}
		}
	}


}

