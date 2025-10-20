/**
 * Author: Me
 * Date: 2025-06-25
 * License: CC0
 * Source: Me
 * Description: Improved hill climbing. Good for small data with clear local maximas. 
 * Consider tweaking temp according to the score scale or running more instances.
 * Status: Stress tested
 */
#pragma once

double simulatedAnnealing(double timeLimit, int n) {
    double temp = 1e5, a = 0.99995, ans = -1e20, cur = -1e20;
    vector<int> w(n), p(n);
    rep(i, 0, n) p[i] = i; // Example: p as permutations
    while (clock() / CLOCKS_PER_SEC <= timeLimit - 0.01) {
        w = p;
        // Example: generate a neighbor by swapping
        int l = rand() % n, r = rand() % n; 
        swap(p[l], p[r]);
        double score = 0; // Calculate score based on p
        ans = max(ans, score);
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
