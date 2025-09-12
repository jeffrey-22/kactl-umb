#include "../utilities/template.h"

#include "../../content/numerical/SimulatedAnnealing.h"

int q[109];

double simulatedAnnealingQ(double timeLimit, int n) {
    double temp = 1e5, a = 0.99995, ans = -1e20, cur = -1e20;
    vector<int> w(n), p(n);
    rep(i, 0, n) p[i] = i; // Example: p as a random permutation
    while (clock() / CLOCKS_PER_SEC <= timeLimit - 0.01) {
        w = p;
        // Example: generate a neighbor by swapping
        int l = rand() % n, r = rand() % n; 
        swap(p[l], p[r]);
        double score = 0; // Calculate score based on p
        rep(i, 0, n) {
            score -= (p[i] - q[i]) * (p[i] - q[i]);
        }
        ans = max(ans, score);
        // cout << cur << endl;
        if (score > cur)
            cur = score;
        else {
            double t = exp((score - cur) / temp);
            int x = 1 + rand(), y = 1 + rand(); x %= y;
            if (1.0 * x / y <= t) cur = score; else p = w;
        }
        temp *= a;
    }
    return ans;
}

int main() {
    // int n = rand() % maxn + 1;
    int n = 100;
    rep(i, 0, n) q[i] = i;
    rep(i, 1, n) swap(q[i], q[rand() % (i + 1)]); // Optimal permutation
    assert (simulatedAnnealingQ(2.0, n) == 0);
	cout<<"Tests passed!"<<endl;
}
