#include <bits/stdc++.h>

using namespace std;

int n;

struct tile {
	int index;
	int price;
	int height;
};

int main() {
	cin >> n;
	vector<tile> front_row(n), back_row(n);
	//读数据，仅按价格排序
	auto read_tile = [&](vector<tile> &tile_row) {
		for (int i = 0; i < n; i++) {
			tile_row[i].index = i + 1;
			cin >> tile_row[i].price;
		}
		for (int i = 0; i < n; i++)
			cin >> tile_row[i].height;
		sort(tile_row.begin(), tile_row.end(), [&](const tile &a, const tile &b) { return a.price < b.price; });
	};
	read_tile(front_row);
	read_tile(back_row);
	//存储当前价格相同的tile
    //auto setCmp=[&](vector<tile>::iterator t1,vector<tile>::iterator t2){return t1->price<t2->price;};
    //height -> iter in front/back row
	map<int,vector<tile>::iterator> front_tile_map, back_tile_map;
	int front_index = 0, back_index = 0;
	vector<int> front_ret(n),back_ret(n);
    int front_ret_index = 0, back_ret_index = 0;
	while (1) {
        //插入当前价格的tile到set
        struct insert_map_arg{
            map<int,vector<tile>::iterator>& tile_map;
            vector<tile>& tile_vec;
            int& vec_index;
        }
        front_map_arg{front_tile_map, front_row, front_index},
        back_map_arg{back_tile_map, back_row, back_index};
        //返回值代表vec是否走到了尽头
        auto insert_map=[&](insert_map_arg& arg)->bool{
            if (arg.tile_map.empty()) {
                int price = arg.tile_vec[arg.vec_index].price;
                if(arg.vec_index < arg.tile_vec.size())
                    return false ;
                while (arg.tile_vec[arg.vec_index].price == price) {
                    front_tile_map.insert(make_pair(arg.tile_vec[arg.vec_index].height, arg.tile_vec.begin() + arg.vec_index));
                    ++arg.vec_index;
                }
            }
            return true;
        };
        bool bF=insert_map(front_map_arg);
        bool bB=insert_map(back_map_arg);
        if(!bF && !bB)
            break;
		//用贪心法，从当前价格区间找出最合适的砖
        if(front_tile_map.size() < back_tile_map.size()){
            for(auto t:front_tile_map){
                int frontH=t.second->height;
                auto iter=back_tile_map.lower_bound(frontH + 1);
                if(iter==back_tile_map.end()){
                    cout<<"impossible"<<endl;
                    return 0;
                }
                front_ret[front_ret_index++]=t.second->index;
                back_ret[back_ret_index++]=iter->second->index;
                back_tile_map.erase(iter);
            }
        } else{
            for(auto t:back_tile_map){
                int backH=t.second->height;
                auto iter=front_tile_map.lower_bound(backH - 1);
                if(iter==front_tile_map.end()){
                    cout<<"impossible"<<endl;
                    return 0;
                }
                front_ret[front_ret_index++]=iter->second->index ;
                back_ret[back_ret_index++]=t.second->index;
                front_tile_map.erase(iter);
            }
        }
	}
    //output answer
    for(auto i:back_ret)
        cout<<i<<" ";
    cout<<endl;
    for(auto i:front_ret)
        cout<<i<<" ";
    cout<<endl;
    return 0;
}













