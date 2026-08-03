#include "../utilities/template.h"

#include "../../content/various/MinPlusConvex.h"

random_device rd;mt19937_64 rng(rd());
uniform_int_distribution<ll> dist(numeric_limits<ll>::min(),numeric_limits<ll>::max());

vector<ll> randomConvexArray(int n) {
    if (n <= 0) return {};
    vector<ll> a(n);
	vector<ll> d(n-1);
	for (auto &v : d) v = dist(rng) / (n * 8 + 1);
	sort(all(d));
	a[0] = dist(rng) / (n * 8 + 1);
	for (int i = 1; i < n; i++) a[i] = a[i - 1] + d[i - 1];
    return a;
}

int main() {
	const int MAX_N_SMALL = 10;
	const int MAX_M_SMALL = 10;
	const int MAX_N_LARGE = 1000;
	const int MAX_M_LARGE = 1000;
	const int iters_SMALL = 1000000;
	const int iters_LARGE = 100;
	rep(it,0,iters_SMALL) {
		int n = rand() % MAX_N_SMALL + 1;
		int m = rand() % MAX_M_SMALL + 1;
		vector<ll>a(n),b(m),c(n+m-1,LLONG_MAX);
		a = randomConvexArray(n);
		for (auto &v : b) v = dist(rng) / 5;
		for (int k = 0; k < n + m - 1; k++) {
			for (int i = 0; i <= k; i++) if (i>=0&&i<n&&k-i>=0&&k-i<m) c[k] = min(c[k], a[i] + b[k - i]);
		}
		assert(c == minPlusConvex(a,b));
	}
	rep(it,0,iters_LARGE) {
		int n = rand() % MAX_N_LARGE + 1;
		int m = rand() % MAX_M_LARGE + 1;
		vector<ll>a(n),b(m),c(n+m-1,LLONG_MAX);
		a = randomConvexArray(n);
		for (auto &v : b) v = dist(rng) / 5;
		for (int k = 0; k < n + m - 1; k++) {
			for (int i = 0; i <= k; i++) if (i>=0&&i<n&&k-i>=0&&k-i<m) c[k] = min(c[k], a[i] + b[k - i]);
		}
		assert(c == minPlusConvex(a,b));
	}
	cout<<"Tests passed!"<<endl;
}
