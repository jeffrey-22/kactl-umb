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

vector<ll> dp(N+1); dp[0] = 0;
vector<pair<int, int> > v = {{0, 0}};
rep(x,1,N+1) { // find dp[x]
	int k = (--lower_bound(all(v), make_pair(x+1, 0)))->second;
	dp[x] = dp[k] + w(k, x);
	for (int i = sz(v) - 1; i >= 0; i--) {
		auto [y, oldk] = v[i];
		if (y > x && dp[x] + w(x, y) < dp[oldk] + w(oldk, y)) 
			v.pop_back();
		else {
			int lo = y+1, hi = N+1;
			while(lo < hi) {
				int mid = (lo+hi)/2;
				if (dp[x] + w(x, mid) <= dp[oldk] + w(oldk, mid))
					hi = mid;
				else 
					lo = mid+1;
			}
			if (hi != N+1) v.push_back({hi, x});
			break;
		}
	}
	if (!sz(v)) v.push_back({0, x});
}