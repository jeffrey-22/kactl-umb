/**
 * Author: mango_lassi on CF
 * Date: 2026-09-08
 * Source: Codeforces
 * Description: Builds a table of a sequence where both rows and columns are increasing. 
 * In case of equal elements, rows are weakly increasing but columns are strongly increasing.
 * Transposing is equal to the Young Tableau of the reverse of the sequence (after forcing strictRow).
 * Sum of first $k$ rows = max size of $k$ disjoint weakly increasing subseq = 
 * longest subseq whose longest strictly decreasing subseq has length $\leq$ k. 
 * Sum of first $k$ cols = max size of $k$ disjoint strictkly decreasing subseq =
 * longest subseq whose longest weakly increasing subseq has length $\leq$ k.
 * RSKMap maps any permutation to a pair $(P,Q)$ and RSKInverse maps it back.
 * Number of odd columns is equal to the number of $i$s that satisfy $a_i=i$ for involution $a$ ($a_{a_i}=i$).
 * Hook-Length formula: \# of Young Tableaus with same shape is $\frac{n!}{\Pi h(x,y)}$
 * where $h(x,y)$ is the \# of squares directly below or to the right of square $(x,y)$.
 * Define comparison on pair $(x,y)$ as first compare $x$ then $y$.
 * Compute $(P,Q)$: inserting $y$ in order of $x$ to get $P$, and record $x$ at inserted position to get $Q$.
 * If reverse every pair to $(y,x)$ and first compare $y$ then x, then it will correspond to $(Q,P)$.
 * Time: $O(NK \log N)$ for buildFirstKRows, $O(N^{1.5} \log N)$ for RSKMap, $O(N^2 \log N)$ for RSKInverse.
 * Status: stress-tested
 */
#pragma once

vector<vi> buildFirstKRows(const vi& a, int k, bool strictRow = false) {
	vector<vi> y(k);
	for (auto c : a)
		rep(i,0,k) {
			int j = (strictRow ? lower_bound(all(y[i]), c) : upper_bound(all(y[i]), c)) - begin(y[i]);
			if (j < sz(y[i])) swap(y[i][j], c);
			else {
				y[i].push_back(c);
				break;
			}
		}
	while(sz(y) && y.back().empty()) y.pop_back();
	return y;
}
// Assumes a is a permutation of 0 - n-1. If not, leave out building q
pair<vector<vi>, vector<vi>> RSKMap(vi a) {
	int n = sz(a), k = 0; assert(n);
	for (;k*k<n;k++);
	vector<vi> p, q, pc, qc; vi c = a;
	p = buildFirstKRows(a, k); reverse(all(a)); 
	pc = buildFirstKRows(a, k, 1); p.resize(sz(pc[0])); 
	vi b(n); rep(i,0,n) b[c[i]] = i;
	q = buildFirstKRows(b, k); reverse(all(b)); 
	qc = buildFirstKRows(b, k); q.resize(sz(qc[0]));
	for (int i = k; i < sz(p); i++)
		for (int j = 0; j < sz(pc) && i < sz(pc[j]); j++) {
			p[i].emplace_back(pc[j][i]);
			q[i].emplace_back(qc[j][i]);
		}
	return {p, q};
}
vi RSKInverse(int n, vector<vi> p, vector<vi> q) {
	vi a(n);
	for (int i = n - 1; i >= 0; i--) {
		int x = 0, y;
		for (; q[x][y = sz(q[x]) - 1] != i; x++);
		int cur = p[x][y];
		p[x].pop_back();
		q[x].pop_back();
		for (x--; x >= 0; x--) {
			y = upper_bound(all(p[x]), cur) - begin(p[x]);
			swap(p[x][y - 1], cur);
		}
		a[i] = cur;
	}
	return a;
}