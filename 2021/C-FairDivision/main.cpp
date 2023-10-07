#include "bits/stdc++.h"

using namespace std;

int main(){
    long long n,m;
    cin>>n>>m;
    // 2^60>10e18, 当N>60, q<2，不存在解
    if(n>60) {
        cout << "impossible" << endl;
        return 0;
    }

    int p, q;
    //brute force遍历可能的pq组合
    for(q=2; ;++q)
        for(p=1;p<q;++p){
            //由 mq/(q^n-(q-p)^n) > mp/(q^n-(q-p)^n), 如果要每个海盗都能拿到整数, 需要mp/(q^n-(q-p)^n)>=1， 当mq/(q^n-(q-p)^n) <1 ，已经没可能有整数解
            if(pow(q,n)-pow(q-p,n)>1.03*m*q){
                if(p!=1)//当前q下找不到整数解了
                    continue;
                else{//如果p=1，再增大q也不可能有整数解了
                    cout << "impossible" << endl;
                    return 0;
                }
            }
            __int128_t qn = 1, qpn = 1;//q和q-p的N次方，double不准确
            for (int i = 0; i < n; i++) qn *= q;
            for (int i = 0; i < n; i++) qpn *= q - p;
            //找到合适的最小的p和q
            if (__int128_t(m) * p % (qn - qpn) == 0) {
                cout<<p<<' '<<q<<endl;
                return 0;
            }
        }
}