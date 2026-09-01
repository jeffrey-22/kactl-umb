/**
 * Author: chilli
 * Date: 2019-05-17
 * License: CC0
 * Source: https://github.com/ngthanhtrung23/ACM_Notebook_new
 * Description: Determine whether a point t lies inside a convex hull (CCW
 * order, with no collinear points). Returns true if point lies within
 * the hull. If strict is true, points on the boundary aren't included.
 * Usage:
 * Status: stress-tested
 * Time: O(\log N)
 */
#pragma once

#include "Point.h"
#include "sideOf.h"
#include "OnSegment.h"

typedef Point<ll> P;

bool inHull(const vector<P>& v, P p, bool strict = true) {
	int a = 1, b = sz(v) - 1, r = !strict;
	if (sz(v) < 3) return r && onSegment(v[0], v.back(), p);
	if (sideOf(v[0], v[a], v[b]) > 0) swap(a, b);
	if (sideOf(v[0], v[a], p) >= r || sideOf(v[0], v[b], p)<= -r)
		return false;
	while (abs(a - b) > 1) {
		int c = (a + b) / 2;
		(sideOf(v[0], v[c], p) > 0 ? b : a) = c;
	}
	return sgn(v[a].cross(v[b], p)) < r;
}
