#include "bits/stdc++.h"

using namespace std;

struct Pnt{
	int x,y,z;
	Pnt(int xx,int yy,int zz):x(xx),y(yy),z(zz){}
	Pnt():x(0),y(0),z(0){}
	Pnt operator+(const Pnt& p2){return Pnt(x+p2.x,y+p2.y,z+p2.z);};
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
	vector<pair<Pnt,Pnt>> rPos;
	map<string,Pnt> fMap={
		{"x+",Pnt(1,0,0)},
		{"y+",Pnt(0,1,0)},
		{"z+",Pnt(0,0,1)},
		{"x-",Pnt(-1,0,0)},
		{"y-",Pnt(0,-1,0)},
		{"z-",Pnt(0,0,-1)},
	};
	for(int i=0;i<k;i++){
		Pnt p1;
		cin>>p1.x>>p1.y>>p1.z;
		p1.x*=2; p1.y*=2; p1.z*=2;
		p1.x+=1; p1.y+=1; p1.z+=1;
		string pos;
		cin>>pos;
		rPos.emplace_back(p1,fMap[pos]);
	}

	return 0;
}