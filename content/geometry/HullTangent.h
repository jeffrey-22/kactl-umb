/**
 * Author: Me
 * Date: 2026-08-22
 * Description: Given a point strictly outside of a convex hull (CCW, no colinear), 
 * return the two tangent points (as indicies of hull vertices) from this point to the hull, 
 * in CCW order from the given point (so right tangent first).
 * Status: not tested
 * Time: O(\log N)
 */
 // TODO: fix and stress test
 #pragma once
 
#include "Point.h"
#include "sideOf.h"

typedef Point<ll> P;

vector<int> hullTangent(const vector<P>& v, P p, bool isLeftTangent) {
    const int n = (int)v.size();
    assert(n >= 3);

    auto cr = [&](int i, int j) -> ll {
        return (v[i] - p).cross(v[j] - p);
    };

    auto onSegment = [&](int a, int b) -> bool {
        if (cr(a, b) != 0) return false;
        return min(v[a].x, v[b].x) <= p.x &&
               p.x <= max(v[a].x, v[b].x) &&
               min(v[a].y, v[b].y) <= p.y &&
               p.y <= max(v[a].y, v[b].y);
    };

    // ------------------------------------------------------------
    // First handle p on the hull.
    //
    // Since p is guaranteed not to be strictly inside, the usual
    // convex-polygon fan search lets us find a boundary edge in O(log n).
    // ------------------------------------------------------------

    auto boundaryResult = [&]() -> vector<int> {
        // p == v[0].
        if (p == v[0]) {
            if (isLeftTangent)
                return {n - 1, 0};
            else
                return {0, 1};
        }

        // Check the two edges incident to v[0].
        if (onSegment(0, 1)) {
            if (p == v[1]) {
                if (isLeftTangent) return {0, 1};
                else return {1, 2 % n};
            }
            return isLeftTangent ? vector<int>{0} : vector<int>{1};
        }

        if (onSegment(n - 1, 0)) {
            if (p == v[n - 1]) {
                if (isLeftTangent) return {n - 2, n - 1};
                else return {n - 1, 0};
            }
            return isLeftTangent
                ? vector<int>{n - 1}
                : vector<int>{0};
        }

        // p must be in the angular wedge v[0] -> p -> v[n-1]
        // for a point inside/on the convex polygon.
        if (v[0].cross(v[1], p) < 0) return {};
        if (v[0].cross(v[n - 1], p) > 0) return {};

        // Find the triangle fan edge containing p.
        int l = 1, r = n - 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (v[0].cross(v[m], p) >= 0)
                l = m;
            else
                r = m;
        }

        if (v[l].cross(v[r], p) != 0)
            return {}; // p is outside, not on the boundary.

        // p lies on v[l] -> v[r].
        if (p == v[l]) {
            if (isLeftTangent)
                return {(l - 1 + n) % n, l};
            else
                return {l, (l + 1) % n};
        }

        if (p == v[r]) {
            if (isLeftTangent)
                return {(r - 1 + n) % n, r};
            else
                return {r, (r + 1) % n};
        }

        if (!onSegment(l, r))
            return {};

        // p is strictly inside the edge l -> r.
        // For a CCW hull, l is the left tangent endpoint and r
        // is the right tangent endpoint.
        return isLeftTangent ? vector<int>{l} : vector<int>{r};
    };

    // If boundaryResult() finds a boundary point, use it.
    // Otherwise p is strictly outside.
    vector<int> boundary = boundaryResult();
    if (!boundary.empty())
        return boundary;

    // ------------------------------------------------------------
    // Strictly outside.
    //
    // Binary-search for the tangent.
    //
    // dir = -1 : left tangent
    // dir = +1 : right tangent
    // ------------------------------------------------------------

    const int dir = isLeftTangent ? -1 : +1;

    auto orient = [&](int a, int b) -> int {
        // orientation of p, v[a], v[b]
        return sgn(cr(a, b));
    };

    function<int(int, int)> tangentSearch =
        [&](int l, int r) -> int {
            // At most two vertices remain.
            if (r - l <= 1) {
                int ans = l;
                for (int i = l + 1; i <= r; ++i) {
                    if (orient(ans, i) != dir)
                        ans = i;
                }
                return ans;
            }

            int m = (l + r) / 2;

            // For a tangent, both neighbours must be on the
            // required side of the line p -> v[m].
            bool prevOK =
                orient(m, (m - 1 + n) % n) != -dir;

            bool nextOK =
                orient(m, (m + 1) % n) != -dir;

            if (prevOK && nextOK)
                return m;

            // Both neighbours are on the wrong side.  The tangent
            // is in one of the two remaining pieces.  Search both
            // pieces and use their orientation to decide which
            // candidate is the desired tangent.
            if (!prevOK && !nextOK) {
                int a = tangentSearch(m + 1, r);
                int b = tangentSearch(l, m - 1);

                return orient(a, b) == dir ? a : b;
            }

            // Previous neighbour is on the wrong side.
            if (!prevOK) {
                if (orient(m, l) == dir)
                    r = m - 1;
                else if (orient(l, r) == dir)
                    r = m - 1;
                else
                    l = m + 1;
            }

            // Next neighbour is on the wrong side.
            if (!nextOK) {
                if (orient(m, l) == dir)
                    l = m + 1;
                else if (orient(l, r) == dir)
                    r = m - 1;
                else
                    l = m + 1;
            }

            return tangentSearch(l, r);
        };

    int t = tangentSearch(0, n - 1);

    // ------------------------------------------------------------
    // If the tangent line coincides with a hull edge, both endpoints
    // are tangent points.  Check the tangent and its two neighbours.
    // ------------------------------------------------------------

    auto isTangent = [&](int i) -> bool {
        int prv = (i - 1 + n) % n;
        int nxt = (i + 1) % n;

        return dir * cr(i, prv) >= 0 &&
               dir * cr(i, nxt) >= 0;
    };

    vector<int> ans;
    for (int d : {-1, 0, 1}) {
        int i = (t + d + n) % n;
        if (isTangent(i))
            ans.push_back(i);
    }

    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());

    return ans;
}