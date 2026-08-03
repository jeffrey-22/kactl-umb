/**
 * Author: Library Checker User Submission
 * Date: 2026-08-03
 * Description: Given CONCAVE a[0..n-1] and arbitrary b[0..m-1], compute c[0..n+m-2] where c[k]=min(a[i]+b[k-i]).
 * Time: O(n\log n)
 * Status: tested
 */
#pragma once

typedef vector<ll> vl; const ll inf = LLONG_MAX/2;
vl minPlusConcave(vl a, vl b) {
	int n=sz(a),m=sz(b);assert(n&&m);
	rep(i,0,n-2)assert(a[i+1]-a[i]>=a[i+2]-a[i+1]);
	vl c(n+m-1,inf);
	auto run=[&](const vl&A,const vl& B,vl& C) {
		int z=sz(A);auto f=[&](int i,int x){return A[x-i]+B[i];};
		auto cross=[&](int i,int j){
			int l=j,r=z,m,q=z;
			while(l<=r)if(f(i,m=l+r>>1)<=f(j,m))q=m,r=m-1;else l=m+1;
			return q;
		};
		vector<pii> h;
		rep(i,0,z) {
			while (sz(h)&&h.back().second<=i)h.pop_back();
			if (!sz(h)||f(h.back().first,i)>f(i,i)){
				int p=z;
				while(sz(h)){
					p=cross(h.back().first,i);
					if(h.back().second<=p)h.pop_back();else break;
				}
				if (!sz(h))h.push_back({i,z+1});
				else h.push_back({i,p});
			}
			C[i]=min(C[i],f(h.back().first,i));
		}
	};
	auto solve=[&](int t){
		vl B(n,inf);for(int i=0;i<n&&t+i<m;i++)B[i]=b[t+i];
		vl d(2*n-1,inf);vl e(n, inf);
		run(a,B,d);vl ar=a,br=B;
		reverse(all(ar));reverse(all(br));
		run(ar,br,e);
		rep(i,0,n) c[t+i]=min(c[t+i],d[i]),
			c[t+2*n-2-i]=min(c[t+2*n-2-i],e[i]);
	};
	for(int i=0;i<m;i+=n)solve(i);
	return c;
}