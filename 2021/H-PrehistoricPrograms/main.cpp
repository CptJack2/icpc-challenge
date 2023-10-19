#include "bits/stdc++.h"

using namespace std;
//stringstream ::cin(R"(
//4
//((
//(
//))
//)))((()
//)");
//fstream cin("/mnt/e/code/icpc-challenge/2021/H-PrehistoricPrograms/data/sample-3.in");
//fstream cin("/mnt/e/code/icpc-challenge/2021/H-PrehistoricPrograms/data/secret-01.in");
//fstream cin("/mnt/e/code/icpc-challenge/2021/H-PrehistoricPrograms/data/secret-more47.in");
struct greed{
    int diff;//左括号和右括号的差
    int maxdiff;//上一个串需要匹配的右括号数量
    int index;//在strs中的index
};
int main() {
    int N;
    ::cin >> N;
    vector<string> strs(N+1);
    int ltot = 0, rtot=0;
    vector<greed> lstr,rstr;
    for(int i=1;i<=N;++i){
        ::cin>>strs[i];
        int l=0,r=0;
        int maxrldiff=0;
        for(auto c:strs[i]) {
            c == '(' ? ++l : ++r;
            maxrldiff=max(maxrldiff,r-l);
        }
        if(l>=r)
            lstr.push_back({l-r, maxrldiff, i});
        else{
            int maxlrdiff=0;
            l=0, r=0;
            for(int j=strs[i].size()-1;j>=0;--j){
                strs[i][j] == '(' ? ++l : ++r;
                maxlrdiff=max(maxlrdiff,l-r);
            }
            rstr.push_back({r-l,maxlrdiff,i});
        }
        ltot+=l;
        rtot+=r;
    }
    //所有串左右括号总和数目不一致
    if(ltot!=rtot) {
        cout << "impossible" << endl;
        return 0;
    }
    auto cmp=[&](const greed& v1, const greed& v2){
        return v1.maxdiff < v2.maxdiff || v1.maxdiff == v2.maxdiff && v1.diff > v2.diff;
    };
    sort(lstr.begin(),lstr.end(),cmp);
    sort(rstr.begin(),rstr.end(),cmp);
    for(int i=0;i<2;++i){
        int cnt=0;
        auto& vec=i?lstr:rstr;
        for (auto v:vec) {
            if (cnt < v.maxdiff) {
                cout << "impossible" << endl;
                return 0;
            }
            cnt += v.diff;
        }
    }
    reverse(rstr.begin(),rstr.end());
    for(auto i:lstr)cout<<i.index<<endl;
    for(auto i:rstr)cout<<i.index<<endl;
}