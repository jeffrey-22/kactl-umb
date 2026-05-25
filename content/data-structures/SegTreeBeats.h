/**
 * Author: Me
 * Date: 2017-10-31
 * License: CC0
 * Source: USACO
 * Description: One-indexed seg-tree, bounds both inclusive. Supports range chmin, chmax, add, sum.
 * Time: O(N \log^2 N)
 * Status: not tested
 */
struct SegTreeBeats {
	struct Node {
		ll sum, max1, max2, // Sum, Max, Second Max
		maxc, min1, min2, minc, lazy; // Max val cnt, Min, Lazy tag
	};
	vector<Node> T; 
	inline static int a = 0, b = 0; // all instances share this index range [a, b]. If this is not desirable, also remove all the default tl = a, tr = b
	const ll inf = 9e18;
	SegTreeBeats(int n) : T(4*n+10) {a = 1, b = n; build();}
	void pushup(int t) {
		int L = (t << 1), R = (t << 1 | 1);
		T[t].sum = T[L].sum + T[R].sum; // sum
		if (T[L].max1 == T[R].max1) { // max
			T[t].max1 = T[L].max1;
			T[t].max2 = max(T[L].max2, T[R].max2);
			T[t].maxc = T[L].maxc + T[R].maxc;
		} else {
			if (T[L].max1 > T[R].max1) {
				T[t].max1 = T[L].max1;
				T[t].max2 = max(T[L].max2, T[R].max1);
				T[t].maxc = T[L].maxc;
			} else {
				T[t].max1 = T[R].max1;
				T[t].max2 = max(T[L].max1, T[R].max2);
				T[t].maxc = T[R].maxc;
			}
		}
		if (T[L].min1 == T[R].min1) { // min
			T[t].min1 = T[L].min1;
			T[t].min2 = min(T[L].min2, T[R].min2);
			T[t].minc = T[L].minc + T[R].minc;
		} else {
			if (T[L].min1 < T[R].min1) {
				T[t].min1 = T[L].min1;
				T[t].min2 = min(T[L].min2, T[R].min1);
				T[t].minc = T[L].minc;
			} else {
				T[t].min1 = T[R].min1;
				T[t].min2 = min(T[L].min1, T[R].min2);
				T[t].minc = T[R].minc;
			}
		}
	}
	void pushadd(int t, int tl, int tr, ll v) {
		if (v == 0) return;
		T[t].sum += v * (tr - tl + 1);
		T[t].max1 += v;
		if (T[t].max2 != -inf) T[t].max2 += v;
		T[t].min1 += v;
		if (T[t].min2 != inf) T[t].min2 += v;
		T[t].lazy += v;
	}
	void pushmax(int t, ll v, bool l) {
		if (v >= T[t].max1) return;
		T[t].sum -= T[t].max1 * T[t].maxc;
		T[t].max1 = v;
		T[t].sum += T[t].max1 * T[t].maxc;
		if (l) T[t].min1 = T[t].max1;
		else {
			if (v <= T[t].min1) T[t].min1 = v;
			else if (v < T[t].min2) T[t].min2 = v;
		}
	}
	void pushmin(int t, ll v, bool l) {
		if (v <= T[t].min1) return;
		T[t].sum -= T[t].min1 * T[t].minc;
		T[t].min1 = v;
		T[t].sum += T[t].min1 * T[t].minc;
		if (l) T[t].max1 = T[t].min1;
		else {
			if (v >= T[t].max1) T[t].max1 = v;
			else if (v > T[t].max2) T[t].max2 = v;
		}
	}
	void pushdown(int t, int tl, int tr) {
		if (tl == tr) return;
		int tm = (tl + tr) >> 1, L = (t << 1), R = (t << 1 | 1);
		pushadd(L, tl, tm, T[t].lazy); // sum
		pushadd(R, tm + 1, tr, T[t].lazy);
		T[t].lazy = 0;
		pushmax(L, T[t].max1, tl == tm); // max
		pushmax(R, T[t].max1, tm + 1 == tr);
		pushmin(L, T[t].min1, tl == tm); // min
		pushmin(R, T[t].min1, tm + 1 == tr);
	}
	void build(int t = 1, int tl = a, int tr = b) {
		T[t].lazy = 0;
		if (tl == tr) {
			T[t].sum = T[t].max1 = T[t].min1 = 0; // or arr[tl]
			T[t].maxc = T[t].minc = 1;
			T[t].max2 = -inf;
			T[t].min2 = inf;
			return;
		}
		int tm = (tl + tr) >> 1;
		build(t << 1, tl, tm);
		build(t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	void update_add(int l, int r, ll v, int t = 1, int tl = a, int tr = b) {
		if (r < tl || tr < l) return;
		if (l <= tl && tr <= r) {
			pushadd(t, tl, tr, v);
			return;
		}
		pushdown(t, tl, tr);
		int tm = (tl + tr) >> 1;
		update_add(l, r, v, t << 1, tl, tm);
		update_add(l, r, v, t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	void update_chmin(int l, int r, ll v, int t = 1, int tl = a, int tr = b) {
		if (r < tl || tr < l || v >= T[t].max1) return;
		if (l <= tl && tr <= r && v > T[t].max2) {
			pushmax(t, v, tl == tr);
			return;
		}
		pushdown(t, tl, tr);
		int tm = (tl + tr) >> 1;
		update_chmin(l, r, v, t << 1, tl, tm);
		update_chmin(l, r, v, t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	void update_chmax(int l, int r, ll v, int t = 1, int tl = a, int tr = b) {
		if (r < tl || tr < l || v <= T[t].min1) return;
		if (l <= tl && tr <= r && v < T[t].min2) {
			pushmin(t, v, tl == tr);
			return;
		}
		pushdown(t, tl, tr);
		int tm = (tl + tr) >> 1;
		update_chmax(l, r, v, t << 1, tl, tm);
		update_chmax(l, r, v, t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	ll query_sum(int l, int r, int t = 1, int tl = a, int tr = b) {
		if (r < tl || tr < l) { return 0; }
		if (l <= tl && tr <= r) { return T[t].sum; }
		pushdown(t, tl, tr);
		int tm = (tl + tr) >> 1;
		return query_sum(l, r, t << 1, tl, tm) + query_sum(l, r, t << 1 | 1, tm + 1, tr);
	}
};
