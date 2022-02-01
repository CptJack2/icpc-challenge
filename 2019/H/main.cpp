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
	auto add_ans=[&](int ind,int v){
	    ans[ind]+=v;
	};
	for(int i=1;i<=n;++i){
		cin>>to[i];
		from[to[i]].push_back(i);
	}
	set<int> visited;
	//每个点遍历,可能存在多个互不连通的独立圈
	for(int node_index=1; node_index <= n; ++node_index){
		if(visited.find(node_index) != visited.end())
			continue;
		int circle_start=node_index;
		//找圈，最小的圈是两个点互相指向
		while(visited.find(to[circle_start])==visited.end()){
			visited.insert(circle_start);
			circle_start=to[circle_start];
		}
        visited.insert(circle_start);
		circle_start=to[circle_start];
		//将圈中所有点放到circle
		vector<int>circle;
		int p_circle=circle_start;
		do {
			circle.push_back(p_circle);
			p_circle=to[p_circle];
		}while(p_circle!=circle_start);
		//用差分表示圆上的值（方便对一段区间做增减）
		int circle_0_abs=0;
		vector<int> circle_diff(circle.size(),0);
		//环上区间增加值，更新差分数组
		auto add_interval=[&](vector<int>::iterator ita,vector<int>::iterator itb,int v){
            //整个圈增加
		    if(ita==circle.begin() && (itb==prev(circle.end()) || itb==circle.end())
		        || ita==next(itb) ){
		        circle_0_abs+=v;
                return ;
		    }
		    int a=ita-circle.begin(),
		        b=itb-circle.begin();
            circle_diff[a]+=v;
            circle_diff[b!=circle.size()-1?b+1:0]-=v;
            //0点在区间内
            if(a>b || a==0)
                circle_0_abs+=v;
		};
		//从圈上找分叉
		for(auto circ_it=circle.begin(); circ_it != circle.end(); ++circ_it){
            //此点不存在分叉
		    if(from[*circ_it].size()<2)
                continue;
		    //找圈上的上一个
			auto circle_prev= prev(circ_it);
			if(circ_it == circle.begin())
				circle_prev=prev(circle.end());
			//广度优先搜索因为无法得知自己是叶子节点，所以只能用深度优先搜索
            //随dfs更新的每层node数量
			vector<int> level_num;
            //当前在k层范围内的node数量总和缓存
			int level_num_sum=0;
			//dfs，对于树的统计，index为node序号，level为树层高
			function<void(int,int)> dfs=[&](int index, int level){
                visited.insert(index);
			    ++level_num_sum;
                int level_num_sum_at_begin=level_num_sum;
                if(level >= level_num.size())
                    level_num.push_back(0);
                ++level_num[level];
                for(auto v:from[index]){
                    //对于根节点要跳过它圈上的上一个
                    if(index==*circ_it && v == *circle_prev)
                        continue;
                    dfs(v, level + 1);
                }
                //超出k层的退栈
                if(level_num.size() > level + k+1){
                    level_num_sum-=level_num.back();
                    level_num.pop_back();
                }
                //dfs到此处,栈中是往后k层的总点数,用开头记录的总点数减去当前总点数,即是k层距离内的总点数
                add_ans(index,level_num_sum - level_num_sum_at_begin);
			};
            dfs(*circ_it,0);
			//树到圈中节点(除树根外)
			auto circ_next=[&](vector<int>::iterator it){
			    if(it!=prev(circle.end()))
			        return next(it);
			    else
			        return circle.begin();
			};
            //树根属于圈上,不计入此处,在后面更新
            level_num_sum-=1;
            auto circ_it2=circ_next(circ_it);
            int k_remain=k-level_num.size()+1;
            if (k_remain > 0) {
                if (k_remain >= circle.size() - 1) {
                    //k的长度超过了树高和整个圈长,除根节点外其余部分增加树中点数，然后可以前进到圈下一点了
                    add_interval(circ_it2, prev(circ_it), level_num_sum);
                    continue;
                } else {
                    //从根起到k_remain部分的点加上树的点数
                    add_interval(circ_it2, circ_it2+k_remain-1, level_num_sum);
                }
            }
            //剩余树逐层退栈
            while(level_num.size()>1 && circ_it2!=circ_it){
                level_num_sum -= level_num.back();
                level_num.pop_back();
                //只剩下根节点
                if(level_num.size()<=1)
                    break;
                add_interval(circ_it2,circ_it2,level_num_sum);
                circ_it2=circ_next(circ_it2);
			}
		}
        //圈中节点互相访问
        int plus=min(k,int(circle.size())-1);
		add_interval(circle.begin(),circle.end(),plus);
		//差分数组还原
		int abs=circle_0_abs;
        for(int i=0;i<circle.size();++i){
            int v=circle[i];
            add_ans(v,abs);
            if(i!=circle.size()-1)
                abs+=circle_diff[i+1];
        }
        int aaa=min(1,2);
	}
	for(int i=1;i<ans.size();++i)
	    cout<<ans[i]<<endl;
}

