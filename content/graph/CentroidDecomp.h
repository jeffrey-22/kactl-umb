/**
 * Author: Me
 * Date: 2025-10-20
 * License: CC0
 * Source: USACO Guide
 * Description: Repeatedly solve from centroid with all children having subtree size <= N/2.
 * Worst case is on a line. ~750ms for N = 5e5
 * Time: $O(N\log N)$
 * Status: Checked at Library Checker
 */
#pragma once

vector<vector<int>> adj;
vector<bool> removed;
vector<int> sz;

int calcSize(int u, int p = -1) {
	sz[u] = 1;
	for (auto v : adj[u]) {
		if (v == p || removed[v]) continue;
		sz[u] += calcSize(v, u);
	}
	return sz[u];
}
int getCentroid(int u, int tot, int p = -1) {
	for (auto v : adj[u]) {
		if (v == p || removed[v]) continue;
		if (sz[v] * 2 > tot) return getCentroid(v, tot, u);
	}
	return u;
}
void centroidDecomp(int u = 0) {
	u = getCentroid(u, calcSize(u));

	// Solve with u as centroid

	removed[u] = 1;
	for (auto v : adj[u]) {
		if (removed[v]) continue;
		centroidDecomp(v);
	}
}