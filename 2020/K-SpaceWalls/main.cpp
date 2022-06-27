#include "bits/stdc++.h"

using namespace std;

struct Pnt{
	int x,y,z;
	Pnt(int xx,int yy,int zz):x(xx),y(yy),z(zz){}
	Pnt():x(0),y(0),z(0){}
	Pnt operator+(const Pnt& p2){return Pnt(x+p2.x,y+p2.y,z+p2.z);};
	int operator*(const Pnt& p2){return x*p2.x+y*p2.y+z*p2.z;}
};

struct Pnt2{
	int x,y;
	Pnt2(int xx,int yy):x(xx),y(yy){}
	Pnt2():x(0),y(0){}
	bool operator!=(const Pnt2& p2){return !(x==p2.x && y==p2.y);}
	Pnt2 operator+(const Pnt2& p2){return Pnt2(x+p2.x,y+p2.y);}
	Pnt2& operator+=(const Pnt2& p2){*this=*this+p2; return *this;}
};

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

	}

	return 0;
}