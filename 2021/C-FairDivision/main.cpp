#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int64_t N, M;
    cin >> N >> M;
    if (N > 200) N = 200;//WTF?

    int p, q;
    vector<double> pw(2);
    for (q = 2;; q++) {
        pw.push_back(pow(q, N));
        for (p = 1; p < q; p++) {
            double d = pw[q] - pw[q - p];
            if (d > 1.1 * M * q) {
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