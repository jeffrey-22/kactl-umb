/**
 * Author: Arpa
 * Date: 2025-10-21
 * License: CC0
 * Source: Codeforces
 * Description: Count how many vertices in subtree of vertex v are colored with color c.
 * Time: $O(N\log N)$
 * Status: Not tested
 */
vector<vector<int>> *ch, adj;
vector<int> cnt, sz;
void dfs(int u, int p, bool keep){
	int maxSz = -1, bigChild = -1;
	for (auto v : adj[u])
		if (v != p && sz[v] > maxSz)
			maxSz = sz[v], bigChild = v;
	for (auto v : adj[u])
		if (v != p && v != bigChild)
			dfs(v, u, 0);
	if (bigChild != -1)
		dfs(bigChild, u, 1), ch[u] = ch[bigChild];
	else
		ch[u] = new vi();
	ch[u]->push_back(u);
	cnt[col[u]]++;
	for (auto v : adj[u])
		if (v != p && v != bigChild)
			for (auto x : *ch[v]) {
				cnt[col[x]]++;
				ch[u]->push_back(x);
			}
	// Now cnt[c] is the number of vertices in subtree of vertex v that has color c.
	// *ch[u] contains all of the subtree nodes of vertex v.
	if (!keep)
		for (auto v : *ch[u])
			cnt[col[v]]--;
}