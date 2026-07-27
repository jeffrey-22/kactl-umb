/**
 * Author: Me
 * Date: 2026-07-17
 * License: CC0
 * Source: USACO
 * Description: One-indexed seg-tree, bounds both inclusive.
 * Time: O(N \log N)
 * Status: not tested
 */
struct Sum {
    ll v;
    Sum(ll vv = 0):v(vv){} // ! Default
};
static Sum merge(Sum lhs, Sum rhs){
    // ! Implement merging
    return Sum(lhs.v+rhs.v);
}
struct Lazy {
    ll v;
    Lazy(ll vv = 0):v(vv){} // ! Default
    void pushlazy(Sum& os, Lazy &ot, int tl, int tr) {
        // ! Push lazy tag *this to child with sum os and lazy ot
        os.v += v * (tr - tl + 1);
        ot.v += v;
    }
};
struct SegTree {
	struct Node {Sum sum; Lazy lazy;};vector<Node> T; 
	inline static int a = 0, b = 0; // all instances share this index range [a, b]. If this is not desirable, also remove all the default tl = a, tr = b
	void init(int n) {T = vector<Node>(4*n+10); a = 1; b = n; build();}
	void pushup(int t) {
		int L = (t << 1), R = (t << 1 | 1);
		T[t].sum = merge(T[L].sum, T[R].sum);
	}
	void pushadd(int t, int tl, int tr, Lazy &lazy) {
        lazy.pushlazy(T[t].sum, T[t].lazy, tl, tr);
	}
	void pushdown(int t, int tl, int tr) {
		if (tl == tr) return;
		int tm = (tl + tr) >> 1, L = (t << 1), R = (t << 1 | 1);
		pushadd(L, tl, tm, T[t].lazy);
		pushadd(R, tm + 1, tr, T[t].lazy);
		T[t].lazy = Lazy();
	}
	void build(int t = 1, int tl = a, int tr = b) {
		T[t].lazy = Lazy();
		if (tl == tr) {
			T[t].sum = Sum(); // or arr[tl]
			return;
		}
		int tm = (tl + tr) >> 1;
		build(t << 1, tl, tm);
		build(t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	void update_add(int l,int r,Lazy v,int t=1,int tl=a,int tr=b){
		if (r < tl || tr < l) return;
		if (l <= tl && tr <= r) {
			pushadd(t, tl, tr, v);
			return;
		}
		pushdown(t, tl, tr); int tm = (tl + tr) >> 1;
		update_add(l, r, v, t << 1, tl, tm);
		update_add(l, r, v, t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	void point_modify(int pos,Sum new_val,int t=1,int tl=a,int tr=b){
		if (tl == tr) {
			T[t].sum = new_val;
			return;
		}
		pushdown(t, tl, tr); int tm = (tl + tr) >> 1;
		if (pos <= tm) point_modify(pos, new_val, t << 1, tl, tm);
		else point_modify(pos, new_val, t << 1 | 1, tm + 1, tr);
		pushup(t);
	}
	Sum query_sum(int l,int r,int t=1,int tl=a,int tr=b){
		if (l <= tl && tr <= r) { return T[t].sum; }
		pushdown(t, tl, tr); int tm = (tl + tr) >> 1;
        if (r <= tm) return query_sum(l, r, t << 1, tl, tm);
        else if (l > tm) return query_sum(l, r, t << 1 | 1, tm + 1, tr);
        else return merge(query_sum(l, r, t << 1, tl, tm), query_sum(l, r, t << 1 | 1, tm + 1, tr));
	}
    void break_range(int l, int r, vector<tuple<int, int, int>>& ranges,int t=1,int tl=a,int tr=b) {
		if (r < tl || tr < l) return;
		if (l <= tl && tr <= r) {ranges.push_back({t,tl,tr});return;}
		pushdown(t, tl, tr); int tm = (tl + tr) >> 1;
        break_range(l, r, ranges, t << 1, tl, tm); break_range(l, r, ranges, t << 1 | 1, tm + 1, tr);
    }
};
