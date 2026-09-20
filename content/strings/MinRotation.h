/**
 * Author: Unknown
 * License: Unknown
 * Source: https://cp-algorithms.com/string/lyndon_factorization.html
 * Description: Finds the lexicographically smallest rotation of a string by Lyndon factorization. 
 A Lyndon word is lexicolly smaller than all of its proper suffixes.
 Any string can be factorized into unique non-increasing Lyndon words $s=w_1\cdots w_k$.
 \texttt{wordStart[i]} stores the starting index of $w_i$.
 * Time: O(N)
 * Usage:
 *  rotate(v.begin(), v.begin()+minRotation(v), v.end());
 * Status: Stress-tested
 */
#pragma once

int minRotation(string s) {
	int n = sz(s), a = 0;
	s += s; // leave out if only factoring
	vector<int> wordStart;
	for (int i = 0, j, k = 0; i < n; k = i) {
		a = i;
		for (j = i + 1; j < sz(s) && s[k] <= s[j]; j++)
			if (s[k] < s[j]) k = i; else k++;
		for (; i <= k; i += j - k) wordStart.push_back(i);
	}
	return a;
}
