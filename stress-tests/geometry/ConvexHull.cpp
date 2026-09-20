#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/IncrementalConvexHull.h"
#include "../../content/geometry/DecrementalConvexHull.h"
#include "../utilities/bench.h"

namespace old {
pair<vi, vi> ulHull(const vector<P>& S) {
	vi Q(sz(S)), U, L;
	iota(all(Q), 0);
	sort(all(Q), [&S](int a, int b){ return S[a] < S[b]; });
	for(auto &it: Q) {
#define ADDP(C, cmp) while (sz(C) > 1 && S[C[sz(C)-2]].cross(\
	S[it], S[C.back()]) cmp 0) C.pop_back(); C.push_back(it);
		ADDP(U, <=); ADDP(L, >=);
	}
	return {U, L};
}

vi convexHull(const vector<P>& S) {
	vi u, l; tie(u, l) = ulHull(S);
	if (sz(S) <= 1) return u;
	if (S[u[0]] == S[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}
}

int main() {
	UpperHull A;
	A.add(P(0,0));
	A.add(P(0,-1));
	A.add(P(-1,0));
	A.add(P(0,-1));
	A.add(P(0,0));
	// for (auto p : A) cerr << p << " "; cerr << endl; 
	// return 0;
	rep(t,0,500) {
		rep(GRID,1,30) {
			rep(SZ,0,30) {
				vector<P> pts(SZ);
				rep(i,0,SZ) pts[i] = P(rand()%GRID - GRID / 2, rand()%GRID - GRID / 2);
				auto res = convexHull(pts);
				auto res2 = old::convexHull(pts);
				assert(sz(res) == sz(res2));
				rep(i,0,sz(res2)) {
					assert(pts[res2[i]] == res[i]);
				}

				UpperHull U, L; U.clear(); L.clear();
				rep(i,0,SZ) {U.add(pts[i]); L.add(pts[i] * (-1));}
				vector<P> uch = {}, lch = {};
				for (auto p : U) uch.push_back(p); reverse(all(uch));
				for (auto p : L) lch.push_back(p * (-1)); reverse(all(lch));
				vector<P> ch = {};
				for (auto p : uch) ch.push_back(p);
				for (auto p : lch) if (!(ch.back() == p) && !(*ch.begin() == p)) ch.push_back(p);
				if (res.empty()) {assert(ch.empty()); continue;}
				int st = -1;
				rep(i, 0, sz(ch)) if (ch[i] == res[0]) {st = i; break;}
				// cerr << "DATA:" << endl;
				// rep(i,0,SZ) cerr << pts[i] << " "; cerr << endl;
				// cerr << "correct hull: "; for (auto t : res) cerr << t << " "; cerr << endl;
				// cerr << "upper hull: "; for (auto t : uch) cerr << t << " "; cerr << endl;
				// cerr << "lower hull: "; for (auto t : lch) cerr << t << " "; cerr << endl;
				// cerr << "dynamic hull: "; for (auto t : ch) cerr << t << " "; cerr << endl;
				assert(st != -1);
				rotate(ch.begin(), ch.begin() + st, ch.end());
				assert(res == ch);
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
