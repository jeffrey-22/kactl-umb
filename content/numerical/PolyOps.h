/**
 * Author: Andrew He
 * Source: github.com/ecnerwala/icpc-book/blob/master/content/numerical/fft.cpp
 * Date: 2026-08-18
 * Description: Polynomial class "For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$"
 */

#include "../number-theory/ModularArithmetic.h"
#include "NumberTheoreticTransform.h"

using Num = Mod; // change to complex numbers if needed
using Poly = vector<Num>;
Poly Multiply(Poly a, Poly b) {
	vl va(sz(a)),vb(sz(b));
	rep(i,0,sz(a))va[i]=a[i].x; rep(i,0,sz(b))vb[i]=b[i].x;
	vl vc=conv(va,vb); Poly c(sz(vc),Num(0));
	rep(i,0,sz(vc))c[i]=Num(vc[i]);
	return c;
}
Poly Reciprocal(Poly a, int n) { // compute first n coeffs
	if (!sz(a) || !n) return {};
	Poly b({Num(1)/a[0]});
	while (sz(b) < n) {
        int m = min(2 * sz(b), n);
		Poly f(a.begin(), a.begin() + min(m, sz(a)));
		Poly c = Multiply(f, b); c.resize(m, Num(0));
		rep(i,0,m) c[i] = Num(0) - c[i]; c[0] = c[0] + Num(2);
		b = Multiply(b, c); b.resize(m, Num(0));
	}
	return b;
}
Poly Differentiate(Poly a) {
	if (!sz(a)) return {};
	Poly b(sz(a)-1, Num(0));
	rep(i,1,sz(a)) b[i-1]=a[i]*Num(i);
	return b;
}
Poly Integrate(Poly a) { // assumes C = 0
	Poly b(sz(a)+1, Num(0));
	rep(i,0,sz(a)) b[i+1]=a[i]/Num(i+1);
	return b;
}
Poly Log(Poly a, int n) { // a[0]=1 must hold
    if (!n) return {};
	Poly b = Integrate(Multiply(Differentiate(a),Reciprocal(a,n-1)));
	b.resize(n, Num(0));
	return b;
}
Poly Exp(Poly a, int n) { // a[0]=0 must hold
    if (!n) return {};
	Poly b = {Num(1)};
	while (sz(b) < n) {
		int m = min(2 * sz(b), n);
		Poly d = Log(b, m); Poly c(m, Num(0));
		rep(i,0,m) c[i] = (i < sz(a) ? a[i] : Num(0)) - d[i];
		c[0] = c[0] + Num(1);
		b = Multiply(b, c); b.resize(m, Num(0));
	}
	return b;
}
Poly Remainder(Poly a, Poly b) {
	if (sz(a) < sz(b)) return a;
	if (!sz(b)) return {};
	int n = sz(a) - sz(b) + 1;
	Poly c(a.rbegin(), a.rend()), d(b.rbegin(), b.rend());
	d = Reciprocal(d, n);
	Poly q = Multiply(c, d); q.resize(n, Num(0));
	reverse(all(q));
	Poly p = Multiply(q, b);
	rep(i,0,sz(p)) a[i] = a[i] - p[i];
	a.resize(sz(b) - 1, Num(0));
	return a;
}
vector<Num> MultipointEvaluate(Poly a, vector<Num> x) {
	int n = sz(x); if (!n) return {};
	int S = 1 << (32 - __builtin_clz(n - 1));
	vector<Poly> t(2 * S, {Num(1)}), r(2 * S);
	rep(i,0,n) t[S + i] = {Num(0) - x[i], Num(1)};
	for (int v = S - 1; v; v--)
		t[v] = Multiply(t[2 * v], t[2 * v + 1]);
	r[1] = Remainder(a, t[1]);
	rep(v,1,S) {
		r[2 * v] = Remainder(r[v], t[2 * v]);
		r[2 * v + 1] = Remainder(r[v], t[2 * v + 1]);
	}
	vector<Num> c = {};
	rep(i,S,S+n) c.push_back(r[i].empty() ? Num(0) : r[i][0]);
	return c;
}
Poly Interpolate(vector<Num> x, vector<Num> y) { // x is distinct
	int n = sz(x); assert(n == sz(y) && n);
	int S = 1 << (32 - __builtin_clz(n - 1));
	vector<Poly> t(2 * S, {Num(1)}), c(2 * S);
	rep(i,0,n) t[S + i] = {Num(0) - x[i], Num(1)};
	for (int v = S - 1; v; v--)
		t[v] = Multiply(t[2 * v], t[2 * v + 1]);
	Poly P = t[1];
	vector<Num> z = MultipointEvaluate(Differentiate(P), x);
	rep(i,0,n) c[S + i] = {y[i] / z[i]};
	for (int v = S - 1; v; v--) {
		Poly a = Multiply(c[2 * v], t[2 * v + 1]);
		Poly b = Multiply(c[2 * v + 1], t[2 * v]);
		c[v].assign(max(sz(a), sz(b)), Num(0));
		rep(i, 0, sz(a)) c[v][i] = c[v][i] + a[i];
		rep(i, 0, sz(b)) c[v][i] = c[v][i] + b[i];
	}
	c[1].resize(n, Num(0));
	return c[1];
}