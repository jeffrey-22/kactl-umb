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
    int n = sz(v); assert(n);
    if (n == 1) return {0, 0};
    if (n == 2) {
        if (sideOf(p, v[0], v[1]) >= 0) return {0, 1};
        return {1, 0};
    }
    // sgn = +1 -> right tangent
    // sgn = -1 -> left tangent
    auto tangent = [&](int sgn) {
        auto at = [&](int i) -> const P& {
            return v[(i + n) % n];
        };
        auto side = [&](int i, int j) {
            return sgn * sideOf(p, at(i), at(j));
        };
        // Vertex 0 may itself be the answer.
        if (side(1, 0) < 0 && side(n - 1, 0) <= 0)
            return 0;
        // Treat index n as another copy of vertex 0.
        int a = 0, b = n;
        while (true) {
            if (b - a == 1) {
                return (side(a, b) > 0 ? a : b) % n;
            }
            int c = (a + b) / 2;
            bool downC = side(c + 1, c) < 0;
            // Both neighbours lie on the correct side:
            // c is the tangent vertex.
            if (downC && side(c - 1, c) <= 0)
                return c;
            bool upA = side(a + 1, a) > 0;
            if (upA) {
                if (downC)
                    b = c;
                else if (side(a, c) > 0)
                    b = c;
                else
                    a = c;
            } else {
                if (!downC)
                    a = c;
                else if (side(a, c) < 0)
                    b = c;
                else
                    a = c;
            }
        }
    };
    return {tangent(+1), tangent(-1)};
}