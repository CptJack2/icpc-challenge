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
		sort(tile_row.begin(), tile_row.end(), [&](const tile &a, const tile &b) { return a.price < b.price });
	};
	read_tile(front_row);
	read_tile(back_row);
	//存储当前价格相同的tile
	set<int> front_tile_set, back_tile_set;
	int front_index = 0, back_index = 0;
	vector<int> front_ret(n),back_ret(n);
	while (1) {
		//插入当前价格的tile到set
		if (front_tile_set.empty()) {
			int price = front_row[front_index].price;
			while (front_index < front_row.size() && front_row[front_index].price == price) {
				front_tile_set.insert(front_row[front_index].index);
				++front_index;
			}
		}
		if (back_tile_set.empty()) {
			int price = back_row[back_index].price;
			while (back_index < back_row.size() && back_row[back_index].price == price) {
				back_tile_set.insert(back_row[back_index].index);
				++back_index;
			}
		}
		//用贪心法，从当前价格区间找出最合适的砖

	}
}