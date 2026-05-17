/**
 * Author: Me
 * Date: 2026-05-17
 * License: CC0
 * Source: cp-algos
 * Description: Given two convex hulls A, B, find the convex hull of A + B
 * Time: O(n + m)
 * Status: Tested on 106252/G
 */
#pragma once

#include "Point.h"

template<class T>
vector<Point<T>> Minko(vector<Point<T>> a, vector<Point<T>> b) {
	rotate(a.begin(), min_element(all(a)), a.end());
	rotate(b.begin(), min_element(all(b)), b.end());
	if (sz(a) < sz(b)) swap(a, b);
	if (sz(b) == 1) {
		for(auto &i : a) i = i + b[0]; return a;
	}
	vector<Point<T>> c; int p1 = 0, p2 = 0;
	while (p1 < sz(a) || p2 < sz(b)) {
		c.push_back(a[p1 % sz(a)] + b[p2 % sz(b)]);
		auto d1 = a[(p1 + 1) % sz(a)] - a[p1];
		auto d2 = b[(p2 + 1) % sz(b)] - b[p2];
		auto dcmp = [&] (Point<T> d1, Point<T> d2) {
			int q1 = d1 == Point<T> {0,0} ? 2 : ((sgn(d1.x) > 0 || (sgn(d1.x) == 0 && sgn(d1.y) > 0)) ? 0 : 1);
			int q2 = d2 == Point<T> {0,0} ? 2 : ((sgn(d2.x) > 0 || (sgn(d2.x) == 0 && sgn(d2.y) > 0)) ? 0 : 1);
			if (q1 != q2) return sgn(q1 - q2); else return -sgn(d1.cross(d2));
		};
		if (p1 < sz(a) && p2 < sz(b) && dcmp(d1,d2) == 0) p1++,p2++;
		else if (p2 == sz(b) || p1 < sz(a) && dcmp(d1,d2) == -1) p1++;
		else p2++;
	}
	return c;
}