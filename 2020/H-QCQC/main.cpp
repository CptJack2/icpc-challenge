#include "bits/stdc++.h"
using namespace std;

//深度优先搜索去匹配
bool dfs(string& A, const string& B, const int index, int availableDigits[10], bool useMin){
	string retStr="";
	//前面的串小于B了,没有最大限制,只需要一路把剩下的数字按大到小排下去
	if(!useMin){
		for(int i=index; i < B.size(); ++i)
			for(int j=9;j>=0;--j)
				if(availableDigits[j]!=0){
					--availableDigits[j],
					A[index]=char(j+'0');
					break;
				}
		return true;
	}
	for(int i=9;i>=0;--i)
		//判断当前可以填的最大数字
		if(availableDigits[i]!=0 &&
			i <= B[index] - '0'){//如果这一位放到最大了,后面的数字都有限制,
			if(index == 0 && i == 0)continue;//首位不能放0
			//更新可用数字的数量
			--availableDigits[i];
			//递归去解
			auto ret=dfs(A,B, index + 1, availableDigits, i == B[index] - '0');
			if(ret){//有解的话就直接返回
				A[index]=char(i+'0');
				return true;
			}else
				//回滚可用数字的数量
				++availableDigits[i];
		}
	return false;
}

string solve(const string& A, const string& B){//输出A排列后最大可能值,无解输出"impossible",要满足A<B
	int availableDigits[10];
	memset(availableDigits,0,sizeof(int)*10);
	string retStr;
	for(auto c:A)
		availableDigits[c-'0']++;
	//A比B长,不可能有解
	if(A.size()>B.size())return "impossible";
	//A比B短,直接数字大到小排下去
	else if(A.size()<B.size()){
		for(int i=0;i<A.size();++i)
			for(int j=9;j>=0;--j)
				if(availableDigits[j]!=0){
					--availableDigits[j],
					retStr+=char(j+'0');
					break;
				}
		return retStr;
	}else if(A.empty())//corner case
		return "";
	//len(A)==len(B),需要递归去解
	retStr=A;
	auto ret=dfs(retStr,B,0,availableDigits,true);
	if(ret)
		return retStr;
	else
		return "impossible";
}

int main(){
	struct testCase{
		string A,B,ans;
	};
	vector<testCase> testCases={
		{"1234","3000","2431"},
		{"387656","387654","387566"},	//中间回溯
		{"766","765","676"},	//开头回溯
		{"","",""},	//AB都空
		{"789","8888","987"},	//A比B短
		{"78900","8888","impossible"},//A比B长
		{"5999","5555","impossible"},//AB等长, 但无解
	};
	for(auto& tc:testCases){
		auto ret=solve(tc.A,tc.B);
		if(ret!=tc.ans)
			cout<<"failed"<<endl;
	}
	cout<<"aaa";
}