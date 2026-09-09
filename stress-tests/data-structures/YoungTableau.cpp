#include "../utilities/template.h"

#include "../../content/data-structures/YoungTableau.h"

#include "../../content/various/LIS.h"

vector<vi> weakRSK(vi a) {
	int n = sz(a), k = 0; assert(n);
	for (;k*k<n;k++);
	vector<vi> p, q, pc, qc; vi c = a;
	p = buildFirstKRows(a, k); reverse(all(a)); 
	pc = buildFirstKRows(a, k, 1); p.resize(sz(pc[0])); 
	for (int i = k; i < sz(p); i++)
		for (int j = 0; j < sz(pc) && i < sz(pc[j]); j++) {
			p[i].emplace_back(pc[j][i]);
		}
	return p;
}

int main() {
	srand(2); mt19937 rnd(rand());
    rep(it,0,150) {
        rep(n,1,40) {
            vi p(n);
            rep(i,0,n) p[i] = i;
            shuffle(all(p), rnd);
            auto [pp,qq] = RSKMap(p);
            auto aa = RSKInverse(n, pp, qq);
            assert(aa == p);
            rep(k,0,n) {
                auto v = buildFirstKRows(p, k);
                assert(sz(v) <= k);
                rep(i,0,k) assert(i >= sz(pp) || v[i] == pp[i]);
            }
            reverse(all(p));
            auto [xx,yy] = RSKMap(p);
            rep(i,0,sz(pp))
                rep(j,0,sz(pp[i]))
                    assert(xx[j][i] == pp[i][j]);            
            vi a(n);
            int vmax = rand() % 5 + 1;
            rep(i,0,n) a[i] = rand() % vmax;
            vi t = lis(a);
            reverse(all(a));
            auto tp = weakRSK(a);
            // cerr << "!" << endl;
            // for (auto v : a) cerr << v << " "; cerr << endl;
            // for (auto row : tp) {for(auto v : row) cerr << v << " "; cerr << endl;}
            // cerr << "!" << endl;
            assert(sz(t) == sz(tp));
        }
    }
	cout<<"Tests passed!"<<endl;
}
