/**
 * Author: Arpa
 * Date: 2025-10-21
 * License: CC0
 * Source: Codeforces
 * Description: Count how many vertices in subtree of vertex u are colored with color c.
 * Time: $O(N \log N)$
 * Status: Not tested
 */
vector<vector<int>> g;
vector<int> cnt, col, sz; 
vector<vector<int>*> ch; // init to size n with null pointers
void sackDFS(int u, int par, bool keep){
	int s = -1; // s: biggest child
	for (auto v : g[u]) 
		if (v != par && (s == -1 || sz[v] > sz[s])) s = v;
	for (auto v : g[u])
		if (v != par && v != s) sackDFS(v, u, 0);
	if (s != -1) sackDFS(s, u, 1), ch[u] = ch[s];
	else ch[u] = new vector<int>();
	ch[u]->push_back(u), cnt[col[u]]++; // add u
	for (auto v : g[u])
		if (v != par && v != s)
			for (auto x : *ch[v])
				ch[u]->push_back(x), cnt[col[x]]++; // add x
	// Now cnt[c] = number of vertices in subtree of vertex u with color c.
	// *ch[u] contains all of the subtree nodes of vertex u.
	if (!keep) for (auto v : *ch[u]) cnt[col[v]]--; // remove v
}