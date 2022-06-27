#include "bits/stdc++.h"

using namespace std;

struct Pnt{
	int x,y,z;
	Pnt(int xx,int yy,int zz):x(xx),y(yy),z(zz){}
	Pnt():x(0),y(0),z(0){}
	Pnt operator+(const Pnt& p2){return Pnt(x+p2.x,y+p2.y,z+p2.z);};
	int operator*(const Pnt& p2){return x*p2.x+y*p2.y+z*p2.z;}
	Pnt operator*(int c){return Pnt(c*x,c*y,c*z);}
};

struct Pnt2{
	int x,y;
	Pnt2(int xx,int yy):x(xx),y(yy){}
	Pnt2():x(0),y(0){}
	bool operator!=(const Pnt2& p2){return !(x==p2.x && y==p2.y);}
	Pnt2 operator+(const Pnt2& p2){return Pnt2(x+p2.x,y+p2.y);}
	Pnt2& operator+=(const Pnt2& p2){*this=*this+p2; return *this;}
};

// Undefined sign for negative inputs
template<typename T> static constexpr T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
template<typename T> static constexpr T wrap_pos(T a, T m) { return a < 0 ? a + m : a; }
template<typename T> static constexpr T sqr(T a) { return a * a; }
// m must be positive
template<typename T> static constexpr T mod(T a, T m) { return wrap_pos(a % m, m); }

template<typename T>
static constexpr T inverse2(T a, T m) { return a <= 1 ? a : mod((1 - inverse2(m % a, a) * m) / a, m); }

// a must be relatively prime to m, m > 0
template<typename T>
static constexpr T inverse(T a, T m) { return inverse2(mod(a, m), m); }
// Solve x = a (mod s) and x = b (mod t)
// Returns -1, -1 if no solutions exist
static pair<long long, long long> crt(long long a, long long s, long long b, long long t){
	assert(0 <= a);
	assert(0 <= b);
	a %= s;
	b %= t;
	long long g = gcd(s, t);
	if (g > 1){
		if (a % g != b % g)
			return {-1, -1};
		long long c, m;
		tie(c, m) = crt(a / g, s / g, b / g, t / g);
		assert(m > 0);
		return {c * g + a % g, m * g};
	}
	else
	{
		long long p = s * t;
		__int128 u = inverse(__int128(s), __int128(t)) * s % p;
		__int128 v = inverse(__int128(t), __int128(s)) * t % p;
		return make_pair((b * u + a * v) % p, p);
	}
}

int main(){
	int n,k;
	cin>>n>>k;
	vector<pair<Pnt,Pnt>> blocks;
	for(int i=0;i<n;i++){
		Pnt p1,p2;
		cin>>p1.x>>p1.y>>p1.z>>p2.x>>p2.y>>p2.z;
		p1.x*=2; p1.y*=2; p1.z*=2;
		p2.x*=2; p2.y*=2; p2.z*=2;
		blocks.emplace_back(p1, p2);
	}
	vector<Pnt> rPos;
	map<string,Pnt> fMap={
		{"x+",Pnt(1,0,0)},
		{"y+",Pnt(0,1,0)},
		{"z+",Pnt(0,0,1)},
		{"x-",Pnt(-1,0,0)},
		{"y-",Pnt(0,-1,0)},
		{"z-",Pnt(0,0,-1)},
	};

	vector<Pnt> F,D;
	for(int i=0;i<k;i++){
		Pnt p1;
		cin>>p1.x>>p1.y>>p1.z;
		p1.x*=2; p1.y*=2; p1.z*=2;
		p1.x+=1; p1.y+=1; p1.z+=1;
		string str;
		cin>>str;
		p1=p1+fMap[str];
		rPos.push_back(p1);
		F.push_back(fMap[str]);
		cin>>str;
		D.push_back(fMap[str]);
	}

	vector<vector<Pnt>> paths(k);
	vector<vector<long long>> times(k);
	for(int i=0;i<k;i++){
		vector<vector<Pnt>> tracks;
		map<int,int> fCords,dCords;
		fCords[INT_MIN]=-1; fCords[INT_MAX]=-1;
		dCords[INT_MIN]=-1; dCords[INT_MAX]=-1;
		Pnt f=F[i],d=D[i];
		Pnt2 initPos{d*rPos[i],f*rPos[i]};
		dCords[initPos.x]=-1;
		fCords[initPos.y]=-1;
		auto tf=[&](int& pi){pi=pi==0?1:0;};
		Pnt ignoredAxis= f + d; tf(ignoredAxis.x); tf(ignoredAxis.y); tf(ignoredAxis.z);

		vector<pair<Pnt2,Pnt2>> rects;
		for(int j=0;j<n;j++){
			if(blocks[j].first * ignoredAxis > rPos[i] * ignoredAxis || blocks[j].second * ignoredAxis < rPos[i] * ignoredAxis)continue;
			int	ldx=blocks[j].first*d,
				ldy=blocks[j].first*f,
				rux=blocks[j].second*d,
				ruy=blocks[j].second*f;
			dCords[ldx]=-1;
			fCords[ldy]=-1;
			dCords[rux]=-1;
			fCords[ruy]=-1;
			if(ldx>rux)swap(ldx,rux);
			if(ldy>ruy)swap(ldy,ruy);
			rects.emplace_back(Pnt2(ldx,ldy),Pnt2(rux,ruy));
		}

		vector<int> fCords2(fCords.size()), dCords2(dCords.size());
		int id=0;
		for(auto & kv:fCords)
			kv.second=id,
			fCords2[id++]=kv.first;
		id=0;
		for(auto & kv:dCords)
			kv.second=id,
			dCords2[id++]=kv.first;

		initPos.x=dCords[initPos.x];
		initPos.y=fCords[initPos.y];
		vector<vector<int>> grid(dCords.size(),vector<int>(fCords.size()));
		for(auto& r:rects){
			r.first.x=dCords[r.first.x];
			r.first.y=fCords[r.first.y];
			r.second.x=dCords[r.second.x];
			r.second.y=fCords[r.second.y];
			grid[r.first.x][r.first.y]++;
			grid[r.second.x][r.second.y]++;
			grid[r.first.x][r.second.y]--;
			grid[r.second.x][r.first.y]--;
		}
		for(int x=0;x<grid.size();x++)
			for(int y=0;y<grid[0].size();y++){
				if(x>0)grid[x][y]+=grid[x-1][y];
				if(y>0)grid[x][y]+=grid[x][y-1];
				if(x>0 && y>0)grid[x][y]-=grid[x-1][y-1];
			}

		vector<Pnt2> segs{initPos};
		Pnt2 pos=initPos;
		int dir=0;
		vector<Pnt2> delta={
			{1,0},
			{0,1},
			{-1,0},
			{0,-1},
		};
		do{
			pos+=delta[dir];
			auto addSeg=[&](){
				int s=segs.size();
				if(s>=2){
					const auto&a=segs[s-2], &b=segs[s-1];
					if(a.x==b.x && b.x==pos.x ||
						a.y==b.y && b.y==pos.y)
						segs.pop_back();
				}
				segs.push_back(pos);
			};
			addSeg();
			vector<Pnt2> side={
				{0,0},
				{-1,0},
				{-1,-1},
				{0,-1},
			};
			auto tL=[&]()->bool{
				auto pt=pos+side[dir];
				return grid[pt.x][pt.y]!=0;
			};
			auto tR=[&]()->bool{
				auto pt=pos+side[(dir+3)&3];
				return grid[pt.x][pt.y]==0;
			};
			if(tL())
				dir=(dir+1)&3;
			else if(tR())
				dir=(dir+3)&3;
		}while(pos!=initPos);

		long long time = -1;
		Pnt prev = {};
		for (const Pnt2 &p : segs){
			int x = dCords2[p.x];
			int y = fCords2[p.y];
			Pnt q = d * x + f * y;
			q=q+ignoredAxis*(rPos[i]*ignoredAxis);
			paths[i].push_back(q);
			auto distance=[&](const Pnt &a, const Pnt &b){
				return abs(a.x-b.x)+abs(a.y-b.y)+abs(a.z-b.z);
			};
			if (time == -1)
				time = 0;
			else
				time += distance(prev, q);
			times[i].push_back(time);
			prev = q;
		}
	}

	long long ans = LLONG_MAX;
	long long hits = 0;
	//每两条轨迹求交
	auto direction=[&](const Pnt &a, const Pnt &b){
		Pnt out;
		out.x = (a.x == b.x) ? 0 : (a.x < b.x) ? 1 : -1;
		out.y = (a.y == b.y) ? 0 : (a.y < b.y) ? 1 : -1;
		out.z = (a.z == b.z) ? 0 : (a.z < b.z) ? 1 : -1;
		return out;
	};
	for (int i = 0; i < k; i++)
		for (int j = i + 1; j < k; j++){
			//轨迹中的每一段拎出来互相判断是否碰撞(相交)
			for (size_t u = 0; u + 1 < paths[i].size(); u++){
				Pnt ai = paths[i][u];
				Pnt bi = paths[i][u + 1];
				Pnt di = direction(ai, bi);
				//轨迹都是平行于坐标轴的,所以两个坐标只有一维不同
				bool upi = ai.x + ai.y + ai.z < bi.x + bi.y + bi.z;
				Pnt li = upi ? ai : bi;
				Pnt hi = upi ? bi : ai;
				for (size_t v = 0; v + 1 < paths[j].size(); v++){
					Pnt aj = paths[j][v];
					Pnt bj = paths[j][v + 1];
					Pnt dj;
					int dotp;
					if (li.x > aj.x && li.x > bj.x)goto skip;
					if (hi.x < aj.x && hi.x < bj.x)goto skip;
					if (li.y > aj.y && li.y > bj.y)goto skip;
					if (hi.y < aj.y && hi.y < bj.y)goto skip;
					if (li.z > aj.z && li.z > bj.z)goto skip;
					if (hi.z < aj.z && hi.z < bj.z)goto skip;
					dj = direction(aj, bj);
					dotp = di*dj;//dotp只可能等于0,1,-1
					if (dotp < 0){//dotp==-1.两段轨迹反方向迎面而来,那么两个机器人运行的轨迹一定是完全重合的,起始点不同,方向相反,一定会碰上.
						// Parallel and in opposite directions
						long long separateDistance = aj * di - ai * di;
						long long t = (separateDistance + times[i][u] + times[j][v]) / 2;
						if (t >= times[i][u] && t <= times[i][u + 1] && //不断两两遍历轨迹中的两段,根据机器人行进到这里的时间,判断是不是碰撞点.
							t >= times[j][v] && t <= times[j][v + 1]){
							hits++;
							ans = min(ans, t);
						}
					} else if (dotp == 0){//两段轨迹有交点,但前进方向是垂直的.
						long long ti = times[i][u] + aj * di - ai * di;
						long long tj = times[j][v] + ai * dj - aj * dj;
						auto solve = crt(ti, times[i].back(), tj, times[j].back());//通过中国剩余定理求出碰撞时间
						if (solve.second > 0){
							hits++;
							ans = min(ans, solve.first);
						}
					}
					//dotp==1的情况,轨迹也是完全一致的.但前进方向相同导致永远不能碰上.
					skip:;
				}
			}
		}
	if (ans == LLONG_MAX)
		cout << "ok\n";
	else
		cout << ans / 2 << '\n';
	
	return 0;
}