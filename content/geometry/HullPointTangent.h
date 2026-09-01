/**
 * Author: Me
 * Date: 2026-08-22
 * Description: Given a point strictly outside of a convex hull (CCW, no colinear), 
 * return the two tangent points (as indicies of hull vertices) from this point to the hull, 
 * in CCW order from the given point (so right tangent first).
 * Status: not tested
 * Time: O(\log N)
 */
 // TODO: stress test
 #pragma once
 
#include "sideOf.h"

typedef Point<ll> P;

pair<int, int> hullPointTangent(const vector<P>& v, P p) {
    assert(sz(v));
    if (sz(v) == 1) return {0, 0};
    int p;
    int a = 0, b = sz(v) - 1;
    while (a < b) {
        int c = (a + b + 1) / 2;
        if (sideOf(p, v[c], v[(c + 1) % sz(v)]) == -1) b = mid - 1; else a = mid;
    }
    p = a;
    a = 0, b = sz(v) - 1;
    while (a < b) {
        int c = (a + b) / 2;
        if (sideOf(p, v[c], v[(c + 1) % sz(v)]) <= 0) b = mid; else a = mid + 1;
    }
    return {p, b};
}