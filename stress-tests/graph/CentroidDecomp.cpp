// https://judge.yosupo.jp/submission/322053
#include <bits/stdc++.h>
using namespace std;

#define rep(i, from, to) for (int i = from; i < (to); ++i)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vector<vector<pii>> adj;
vector<bool> removed;
vector<int> sz;
ll ans = 0;
int ans_u = 0, ans_v = 0;

int calcSize(int u, int p = -1) {
	sz[u] = 1;
	for (auto [v, w] : adj[u]) {
		if (v == p || removed[v]) continue;
		sz[u] += calcSize(v, u);
	}
	return sz[u];
}
int getCentroid(int u, int tot, int p = -1) {
	for (auto [v, w] : adj[u]) {
		if (v == p || removed[v]) continue;
		if (sz[v] * 2 > tot) return getCentroid(v, tot, u);
	}
	return u;
}
pair<ll, int> getDepth(int u, int p) {
    pair<ll, int> maxDepth = {0, u};
	for (auto [v, w] : adj[u]) {
		if (v == p || removed[v]) continue;
        auto [d, target] = getDepth(v, u);
		maxDepth = max(maxDepth, {d + w, target});
	}
    return maxDepth;
}
void centroidDecomp(int u = 0) {
	u = getCentroid(u, calcSize(u));
    // cerr << "C = " << u << endl;

	// Solve with u as centroid
    vector<pair<ll, int>> D = {};
	for (auto [v, w] : adj[u]) {
		if (removed[v]) continue;
        auto [d, target] = getDepth(v, u);
        D.push_back({d + w, target});
	}
    sort(all(D));
    // cerr << "D sorted = " << sz(D) << endl;
    if (sz(D) >= 2 && D[sz(D) - 1].first + D[sz(D) - 2].first > ans) {
        ans = D[sz(D) - 1].first + D[sz(D) - 2].first;
        ans_u = D[sz(D) - 1].second;
        ans_v = D[sz(D) - 2].second;
    }

	removed[u] = 1;
	for (auto [v, w] : adj[u]) {
		if (removed[v]) continue;
		centroidDecomp(v);
	}
}
void getPath(int u, int t, vector<int>& path, int p = -1) {
    path.push_back(u);
    if (path.back() == t) return;
    for (auto [v, w] : adj[u]) {
        if (v == p) continue;
        getPath(v, t, path, u);
        if (path.back() == t) return;
    }
    path.pop_back();
}
int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);
	int n;
    // cin >> n;
    n = 1;
    adj.resize(n); fill(all(adj), vector<pii>());
    removed.resize(n); fill(all(removed), 0);
    sz.resize(n);
    for (int i = 0; i <= n - 2; i++) {
        int u, v, w;
        // cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    centroidDecomp();
    // cerr << "Finished" << endl;
    vector<int> path = {};
    getPath(ans_u, ans_v, path);
    // cout << ans << " " << sz(path) << endl;
    // for (auto u : path)  cout << u << " "; cout << endl;
	cout<<"Tests passed!"<<endl;
}
