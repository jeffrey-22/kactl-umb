#include "../utilities/template.h"
#include "../../content/data-structures/SegTreeBeats.h"

int main() {
    for (int it = 0; it < 1000; it++) {
        int n = rand() % 10 + 1;
        if (it >= 990) n = rand() % 10000 + 1;
        SegTreeBeats T = SegTreeBeats(n);
        vector<ll> a(n + 1, 0ll);
        for (int q = 0; q < 5000; q++) {
            int L = rand() % n + 1, R = rand() % n + 1;
            ll val = rand() % 100 + 1;
            int type = rand() % 4 + 1; if (rand() % 2 == 1) type = 4;
            if (type == 1) {
                T.update_add(L, R, val);
                for (int i = L; i <= R; i++) a[i] += val;
            }
            else if (type == 2) {
                T.update_chmin(L, R, val);
                for (int i = L; i <= R; i++) a[i] = min(a[i], val);
            }
            else if (type == 3) {
                T.update_chmax(L, R, val);
                for (int i = L; i <= R; i++) a[i] = max(a[i], val);
            }
            else {
                ll sa = 0;
                for (int i = L; i <= R; i++) sa += a[i];
                assert(T.query_sum(L, R) == sa);
            }
        }
    }
    SegTreeBeats T = SegTreeBeats(3);
    T.update_add(1, 3, 1000000000000ll);
    assert(T.query_sum(1, 3) == 3000000000000ll);
	cout<<"Tests passed!"<<endl;
}