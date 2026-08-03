/**
 * Author: Me
 * Date: 2017-10-31
 * License: CC0
 * Source: USACO
 * Description: One-indexed seg-tree, bounds both inclusive. Supports range chmin, chmax, add, sum.
 * Time: O(N \log^2 N)
 * Status: not tested
 */
struct SegTreeBeats {
	#define AX auto &x=T[t]
	#define PU pushup(t)
	#define PD pushdown(t, tl, tr)
	#define TM int tm=(tl+tr)/2
	#define LR int L=t<<1,R=L|1
	#define DJ r < tl || tr < l
	#define SBS l <= tl && tr <= r
	#define LC l, r, v, L, tl, tm
	#define RC l, r, v, R, tm + 1, tr	
	struct Node {
		ll sum, max1, max2, // Sum, Max, Second Max
		maxc, min1, min2, minc, lazy; // Max val cnt, Min, Lazy tag
	};
	vector<Node> T; 
	inline static int a=0, b=0; // all instances share this index range [a, b]. If this is not desirable, also remove all the default tl = a, tr = b
	const ll inf = 9e18;
	SegTreeBeats(int n) : T(4*n+10) {a=1, b=n; build();}
	void pushup(int t) {
		LR; AX;
		x.sum=T[L].sum+T[R].sum; // sum
		if(T[L].max1<T[R].max1)swap(L,R); // max
		auto &u=T[L],&v=T[R];x.max1=u.max1;
		x.maxc=u.max1==v.max1?u.maxc+v.maxc:u.maxc;
		x.max2=u.max1==v.max1?max(u.max2,v.max2):max(u.max2,v.max1);
		if(T[L].min1>T[R].min1)swap(L,R); // min
		auto &y=T[L],&z=T[R];x.min1=y.min1;
		x.minc=y.min1==z.min1?y.minc+z.minc:y.minc;
		x.min2=y.min1==z.min1?min(y.min2,z.min2):min(y.min2,z.min1);
	}
	void pushadd(int t, int tl, int tr, ll v) {
		AX;if(v==0)return;
		x.sum+=v*(tr-tl+1);x.lazy+=v;
		x.max1+=v;x.max2+=(x.max2!=-inf)?v:0;	
		x.min1+=v;x.min2+=(x.min2!=inf)?v:0;
	}
	void pushmax(int t, ll v, bool l) {
		AX;if(v>=x.max1)return;
		x.sum+=(v-x.max1)*x.maxc;x.max1=v;
		if(l)x.min1=x.max1;else{
			if(v<=x.min1)x.min1=v;
			else x.min2=min(v,x.min2);
		}
	}
	void pushmin(int t, ll v, bool l) {
		AX;if(v<=x.min1)return;
		x.sum+=(v-x.min1)*x.minc;x.min1=v;
		if(l)x.max1=x.min1;else{
			if(v>=x.max1)x.max1=v;
			else x.max2=max(v,x.max2);
		}
	}
	void pushdown(int t, int tl, int tr) {
		if(tl==tr)return;TM;LR;AX;
		pushadd(L,tl,tm,x.lazy); // sum
		pushadd(R,tm+1,tr,x.lazy); x.lazy=0;
		pushmax(L,x.max1,tl==tm); // max
		pushmax(R,x.max1,tm+1==tr);
		pushmin(L,x.min1,tl==tm); // min
		pushmin(R,x.min1,tm+1==tr);
	}
	void build(int t = 1, int tl = a, int tr = b) {
		AX;x.lazy=0;
		if(tl==tr){
			x.sum=x.max1=x.min1=0; // or arr[tl]
			x.maxc=x.minc=1;
			x.max2=-inf;x.min2=inf;
			return;
		}
		TM;LR;build(L,tl,tm);build(R,tm+1,tr);PU;
	}
	void update_add(int l,int r,ll v,int t=1,int tl=a,int tr=b){
		if(DJ)return;if(SBS){pushadd(t,tl,tr,v);return;}
		PD;TM;LR;update_add(LC);update_add(RC);PU;
	}
	void update_chmin(int l,int r,ll v,int t=1,int tl=a,int tr=b){
		if(DJ||v>=T[t].max1)return;
		if(SBS&&v>T[t].max2){pushmax(t,v,tl==tr);return;}
		PD;TM;LR;update_chmin(LC);update_chmin(RC);PU;
	}
	void update_chmax(int l,int r,ll v,int t=1,int tl=a,int tr=b){
		if(DJ||v<=T[t].min1)return;
		if(SBS&&v<T[t].min2){pushmin(t,v,tl==tr);return;}
		PD;TM;LR;update_chmax(LC);update_chmax(RC);PU;
	}
	ll query_sum(int l,int r,int t=1,int tl=a,int tr=b){
		if(DJ)return 0;if(SBS)return T[t].sum;PD;TM;LR;
		return query_sum(l,r,L,tl,tm)+query_sum(l,r,R,tm+1,tr);
	}
};
