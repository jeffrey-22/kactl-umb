/**
 * Author: Simon Lindholm
 * License: CC0
 * Source: Codeforces
 * Description: $dp[0]=0$, computes $dp[i]=\min_{0 \le j < i}dp[j]+w(j,i)$
 * up to $dp[N]$, where $w(j,i)$ satisfies quadrangle inequality.
 * Time: O(N \log N)
 * Status: NOT TESTED
 */
#pragma once

ll w(int l, int r) {
	return (r - l + 1); // cover all 0 <= l <= r <= n cases
}
vector<ll> dp1d1d(int n) {
	vector<ll> dp(++n, 0ll);
	vector<pair<int, int>> v = {{0, 0}};
	rep(x, 1, n) { // find dp[x]
		auto k = get<1>(*(--lower_bound(all(v), make_pair(x + 1, 0))));
		dp[x] = dp[k] + w(k, x);
		for (int i = sz(v) - 1; i >= 0; i--) {
			auto [y, t] = v[i];
			if (y > x && dp[x] + w(x, y) < dp[t] + w(t, y)) 
				v.pop_back();
			else {
				int lo = y + 1, hi = n;
				while(lo < hi) {
					int mid = (lo + hi)/2;
					if (dp[x] + w(x, mid) <= dp[t] + w(t, mid))
						hi = mid;
					else 
						lo = mid + 1;
				}
				if (hi != n) v.push_back({hi, x});
				break;
			}
		}
		if (!sz(v)) v.push_back({0, x});
	}
	return dp;
}