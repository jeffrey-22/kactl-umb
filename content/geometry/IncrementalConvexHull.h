/**
 * Author: brunomont
 * Date: 2022
 * License: Unlicense
 * Source: https://codeforces.com/blog/entry/112458
 * Description: Maintain upper layer of an incremental convex hull.
 * Time: O(\log n)
 * Usage:
 * UpperHull U, L; U.add(p); L.add(p * (-1));
*/
#pragma once

#include "Point.h"
#include "sideOf.h"

typedef Point<ll> P;
struct UpperHull : set<P> {
	int aboveHull(P p) { // return -1/0/1 for below/border/above
		auto a = lower_bound(p);
		if (a == end()) return 1;
		if (a == begin()) return p == *a ? 0 : p.x == a->x ? -1 : 1;
        return sideOf(*a, p, *prev(a));
	}
	void add(P p) {
		if (aboveHull(p) < 1) return;
        auto a = lower_bound(p);
		while (a != end() && next(a) != end() && sideOf(*next(a), *a, p) <= 0)
			a = erase(a);
		while (a != begin() && p.x == prev(a)->x) a = erase(--a);
		while (a != begin() && --a != begin() && sideOf(p, *a, *prev(a)) <= 0)
			a = erase(a);
		insert(p);
	}
};