/**
 * Author: Andrew He
 * Source: github.com/ecnerwala/icpc-book/blob/master/content/numerical/fft.cpp
 * Date: 2026-08-18
 * Description: Polynomial class "For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$"
 */

#include "../number-theory/ModularArithmetic.h"
#include "NumberTheoreticTransform.h"

using Num = Mod; // change to use complex numbers if needed
using Poly = vector<Num>;
Poly Multiply(Poly a, Poly b) {
	vl va(sz(a)),vb(sz(b));
	rep(i,0,sz(a))va[i]=a[i].x;
	rep(i,0,sz(b))vb[i]=b[i].x;
	vl vc=conv(va,vb);
	Poly c(sz(vc),Num(0));rep(i,0,sz(vc))c[i]=Num(vc[i]);
	return c;
}
Poly Reciprocal(Poly a, int n) { // compute first n coeffs
	if (a.empty() || !n) return {};
	Poly b({Num(1)/a[0]});
	while (sz(b) < n) {
        int m = min(2 * sz(b), n);
		Poly f(a.begin(), a.begin() + min(m, sz(a)));
		Poly c = Multiply(f, b);
		c.resize(m, Num(0));
		rep(i,0,m) c[i]=Num(0)-c[i];
		c[0]=c[0]+Num(2);
		b = Multiply(b, c);
		b.resize(m, Num(0));
	}
	return b;
}
Poly Differentiate(Poly a) {
	if (a.empty()) return {};
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
		Poly lb = Log(b, m);
		Poly c(m, Num(0));
		rep(i, 0, m) {
			c[i] = (i < sz(a) ? a[i] : Num(0)) - lb[i];
		}
		c[0] = c[0] + Num(1);
		b = Multiply(b, c);
		b.resize(m, Num(0));
	}
	return b;
}
Poly Remainder(Poly a, Poly b) {
	if (a.empty() || sz(a) < sz(b)) return a;
	if (b.empty()) return {};
	int n = sz(a) - sz(b) + 1;
	Poly ra(a.rbegin(), a.rend());
	Poly rb(b.rbegin(), b.rend());
	rb = Reciprocal(rb, n);
	Poly q = Multiply(ra, rb);
	q.resize(n, Num(0));
	reverse(q.begin(), q.end());
	Poly prod = Multiply(q, b);
	a.resize(sz(a), Num(0));
	rep(i, 0, sz(prod))
		a[i] = a[i] - prod[i];
	a.resize(sz(b) - 1, Num(0));
	return a;
}
// Multipoint evaluation/interpolation
// Given polynomial a, evaluate at x
vector<Num> MultiEval(Poly a, vector<Num> x) {
	int n = sz(x);
	if (!n) return {};
	int S = 1;
	while (S < n) S <<= 1;
	vector<Poly> tree(2 * S);
	rep(i, 0, S)
		if (i < n)
			tree[S + i] = {Num(0) - x[i], Num(1)};
		else
			tree[S + i] = {Num(1)};
	for (int v = S - 1; v; --v)
		tree[v] = Multiply(tree[2 * v], tree[2 * v + 1]);
	vector<Num> ans(n, Num(0));
	vector<Poly> rem(2 * S);
	rem[1] = Remainder(a, tree[1]);
	for (int v = 1; v < S; ++v) {
		rem[2 * v] = Remainder(rem[v], tree[2 * v]);
		rem[2 * v + 1] = Remainder(rem[v], tree[2 * v + 1]);
	}
	rep(i, 0, n)
		ans[i] = rem[S + i].empty() ? Num(0) : rem[S + i][0];
	return ans;
}
// Given points (x_i, y_i), interpolate to get a polynomial
Poly Interpolate(vector<Num> x, vector<Num> y) {
	int n = sz(x);
	assert(n == sz(y));
	assert(n);
	int S = 1;
	while (S < n) S <<= 1;
	vector<Poly> tree(2 * S);
	rep(i, 0, S) {
		if (i < n)
			tree[S + i] = {Num(0) - x[i], Num(1)};
		else
			tree[S + i] = {Num(1)};
	}
	for (int v = S - 1; v; --v)
		tree[v] = Multiply(tree[2 * v], tree[2 * v + 1]);
	Poly P = tree[1];
	Poly dP = Differentiate(P);
	vector<Num> dPx = MultiEval(dP, x);
	vector<Poly> cur(2 * S);
	rep(i, 0, n) {
		assert(dPx[i].x != 0); // x_i must be distinct
		cur[S + i] = {y[i] / dPx[i]};
	}
	for (int v = S - 1; v; --v) {
		Poly left = Multiply(cur[2 * v], tree[2 * v + 1]);
		Poly right = Multiply(cur[2 * v + 1], tree[2 * v]);
		int m = max(sz(left), sz(right));
		cur[v].assign(m, Num(0));
		rep(i, 0, sz(left))
			cur[v][i] = cur[v][i] + left[i];
		rep(i, 0, sz(right))
			cur[v][i] = cur[v][i] + right[i];
	}
	cur[1].resize(n, Num(0));
	return cur[1];
}