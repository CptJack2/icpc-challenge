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

	vector<vector<Pnt>> tracks;
	map<int,int> fCords,dCords;
	for(int i=0;i<k;i++){
		Pnt f=F[i],d=D[i];
		auto tf=[&](int& pi){pi=pi==0?1:0;};
		Pnt ignoredAxis= f + d; tf(ignoredAxis.x); tf(ignoredAxis.y); tf(ignoredAxis.z);

		vector<pair<Pnt2,Pnt2>> rects;
		for(int j=0;j<n;j++){
			if(blocks[j].first * ignoredAxis > rPos[i] * ignoredAxis || blocks[j].second * ignoredAxis < rPos[i] * ignoredAxis)continue;
			int ldx=blocks[i].first*f,
				ldy=blocks[i].first*d,
				rux=blocks[i].second*f,
				ruy=blocks[i].second*d;
			fCords[ldx]=-1;
			dCords[ldy]=-1;
			fCords[rux]=-1;
			dCords[ruy]=-1;
			if(ldx>rux)swap(ldx,rux);
			if(ldy>ruy)swap(ldy,ruy);
			rects.emplace_back(Pnt2(ldx,ldy),Pnt2(rux,ruy));
		}

		vector<int> fCords2(fCords.size()), dCords2(dCords.size());
		int id=0;
		for(auto & kv:fCords)
			fCords2[id++]=kv.first,
			kv.second=id;
		id=0;
		for(auto & kv:dCords)
			dCords2[id++]=kv.first,
			kv.second=id;
		for(auto& r:rects){
			r.first.x=fCords[r.first.x];
			r.first.y=dCords[r.first.y];
			r.second.x=fCords[r.second.x];
			r.second.y=dCords[r.second.y];
		}

		vector<vector<int>> grid(fCords.size(),vector<int>(dCords.size()));

	}



	return 0;
}