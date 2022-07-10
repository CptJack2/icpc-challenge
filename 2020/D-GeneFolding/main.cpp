/*经过一些修改的Manacher算法,用贪心的思路不断缩短两头的回文串
 *这鸟题的问题描述有问题*/
#include "bits/stdc++.h"
using namespace std;
//014.in
string gene;

inline void makeMGene(string& mGene){
	mGene.resize(2 * gene.size() + 1);
	int i = 0;
	for (auto c:gene)
		mGene[2 * i] = '#',
		mGene[2 * i + 1] = c,
		++i;
	mGene.back() = '#';
}

inline void loop(int& i, vector<int>& radius, string& mGene){
	while (i + radius[i] < mGene.size() && i - radius[i] >= 0 && mGene[i + radius[i]] == mGene[i - radius[i]])
		++radius[i];
}

inline void if1(int& i,vector<int>& radius, int& rBorder, int& center){
	if (i + radius[i] - 1 > rBorder)
		rBorder = i + radius[i] - 1,
		center = i;
}

inline void if2(int& i, vector<int>& radius, int& head){
	if (i % 2 == 0 && radius[i] - 1 >= i - head)//char is a '#' and it can reach head
		head = i;
}

inline void getRadius(string& mGene, vector<int>& radius, int& head, int& tail){
	int rBorder = -1, center = -1;
	for (int i = 0; i < mGene.size(); ++i) {
		radius[i] = rBorder >= i ? min(radius[center + center - i], rBorder - i) : 1;
		loop(i,radius,mGene);
		if1(i,radius,rBorder,center);
		if2(i,radius,head);
	}
}

inline void getAns(string& mGene, vector<int>& radius, int& head, int& tail, int& ans){
	for (int i = mGene.size() - 1; i >= head; i -= 2)
		if (radius[i] - 1 >= tail - i)
			tail -= min(i - head, tail - i);//超出头的部分不给它剪了
	ans = (tail - head) / 2;
}

int main(){
	cin>>gene;
	for(int ii=0;ii<150;ii++){
		string mGene;
		makeMGene(mGene);

		vector<int> radius(mGene.size(), 0);
		int head = 0, tail = mGene.size() - 1;//答案的头和尾在的位置
		getRadius(mGene,radius,head,tail);

		int ans=0;
		getAns(mGene,radius,head,tail,ans);

		if(ii%5==0)
			cout<<ii<<" round done"<<endl;
	}
//	cout<<ans;
}