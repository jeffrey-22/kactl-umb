#include "../utilities/template.h"

#include "../../content/numerical/PolyOps.h"

int main() {
    for (int it = 0; it < 100; it++) {
        int n = rand() % 10 + 1;
        Poly a(n, Num(0)); 
        int m = rand() % 10 + 1;
        Poly b(m, Num(0));
        for (int i = 0; i < n; i++) a[i] = Num(rand() % mod);
        for (int i = 0; i < m; i++) b[i] = Num(rand() % mod);
        Poly naivec(n+m-1, Num(0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                naivec[i + j] = naivec[i + j] + a[i] * b[j];
        Poly c = Multiply(a, b);
        assert(sz(c) == sz(naivec));
        for (int i = 0; i < n + m - 1; i++) assert(c[i].x == naivec[i].x);
        if (a[0].x == 0) a[0] = Num(1);
        Poly reca = Reciprocal(a, n);
        Poly prod = Multiply(a, reca);
        assert(prod[0].x == 1);
        for (int i = 1; i < sz(a); i++) assert(prod[i].x == 0);
        a[0] = Num(0);
        Poly a2 = Integrate(Differentiate(a));
        Poly a3 = Differentiate(Integrate(a));
        Poly a4 = Log(Exp(a, n + 1), n);
        assert(sz(a) == sz(a2) && sz(a) == sz(a3) && sz(a) == sz(a4));
        for (int i = 0; i < sz(a); i++) assert(a[i].x == a2[i].x && a[i].x == a3[i].x && a[i].x == a4[i].x);
        vector<Num> res = MultiEval(a, b);
        assert(sz(res) == sz(b));
        for (int i = 0; i < m; i++) {
            Num naiveres = Num(0);
            for (int j = 0; j < n; j++) naiveres = naiveres + a[j] * (b[i] ^ j);
            assert(naiveres.x == res[i].x);
        }
        m = n;
        b = Poly(m, Num(0));
        b[0] = Num(rand() % mod);
        for (int i = 1; i < m; i++) {
            b[i] = Num(rand() % mod);
            while (1) {
                int same = 0;
                for (int j = 0; j < i; j++) if (b[j].x == b[i].x) {same = 1; break;}
                if (!same) break; b[i] = Num(rand() % mod);
            }
        }
        Poly a5 = Interpolate(b, MultiEval(a, b));
        assert(sz(a5) == sz(a));
        for (int i = 0; i < sz(a); i++) assert(a[i].x == a5[i].x);
    }
	cout<<"Tests passed!"<<endl;
}