#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int64_t N, M;
    cin >> N >> M;
    if (N > 60) N = 60;// 2^60>10e18, 当N>60, (p+q)<=2

    int p, q;
    vector<double> pw(2);
    for (q = 2;; q++) {
        pw.push_back(pow(q, N));
        for (p = 1; p < q; p++) {
            double d = pw[q] - pw[q - p];
            if (d > 1.03 * M * q) {// mq/d > mp/d, 如果要每个海盗都能拿到整数, 需要mp/d>=1， 当mq/d <1 ，已经没可能有整数解
                if (p == 1) goto fail;
                continue;
            }
            __int128_t qp = 1, pp = 1;
            for (int i = 0; i < N; i++) qp *= q;
            for (int i = 0; i < N; i++) pp *= q - p;
            if (__int128_t(M) * p % (qp - pp) == 0) goto done;
        }
    }

    done:
    cout << p << ' ' << q << endl;
    return 0;
    fail:
    cout << "impossible" << endl;
}