/**
 * Author: Library Checker User Submission
 * Date: 2026-08-03
 * Description: Given \textbf{convex} \texttt{a[0..n-1]} and arbitrary \texttt{b[0..m-1]}, 
 compute \texttt{c[0..n+m-2]} where \texttt{c[k]=min(a[i]+b[k-i])}.
 * Time: O(n\log n)
 * Status: tested
 */
#pragma once

vector<ll> minPlusConvex(vector<ll> a, vector<ll> b) {
	int n=sz(a),m=sz(b),z=n+m-1; assert(n&&m);
	rep(i,0,n-2) assert(a[i+1]-a[i]<=a[i+2]-a[i+1]);
	vector<ll> c(z,LLONG_MAX); vector<int> e(z+1);
	c[0]=a[0]+b[0], e[z]=m-1; int d=1; while(d<z) d*=2;
	for(int q=d/2;q>0;q/=2) for(int h=q;h<z;h+=q*2) {
		int l=h-q, r=min(h+q,z); e[h]=e[l];
		rep(t,e[l],e[r]+1)
			if(t<=h && h-t<n && c[h]>b[t]+a[h-t])
				c[h]=b[t]+a[h-t],e[h]=t;
	}
	return c;
}