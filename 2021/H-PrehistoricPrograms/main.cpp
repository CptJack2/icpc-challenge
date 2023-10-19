#include "bits/stdc++.h"

using namespace std;
struct greed{
    int surplus;//顺差,左括号比右括号多的数量
    int deficit;//逆差,上一个串需要匹配的右括号数量
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
    //先按逆差从小到大排序, 然后顺差从大到小
    auto cmp=[&](const greed& v1, const greed& v2){
        return v1.deficit < v2.deficit || v1.deficit == v2.deficit && v1.surplus > v2.surplus;
    };
    sort(lstr.begin(),lstr.end(),cmp);
    //右串可以当成从右往左读,以达到和左串同样的贪心处理
    sort(rstr.begin(),rstr.end(),cmp);
    for(int i=0;i<2;++i){
        int cnt=0;
        auto& vec=i?lstr:rstr;
        for (auto v:vec) {
            if (cnt < v.deficit) {
                cout << "impossible" << endl;
                return 0;
            }
            cnt += v.surplus;
        }
    }
    reverse(rstr.begin(),rstr.end());
    for(auto i:lstr)cout<<i.index<<endl;
    for(auto i:rstr)cout<<i.index<<endl;
}