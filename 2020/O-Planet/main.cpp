#include <bits/stdc++.h>
using namespace std;

struct Point{
	int longitude;//(0,360)
	int latitude;//(0,180)
	int deltaLongitude;//difference of longitude between this point and previous
};

bool operator==(const Point& a, const Point& b){
	return a.deltaLongitude==b.longitude && a.latitude==b.latitude;
}

constexpr int scale=10000;

int main(){
	//read in data
	int n;
	cin>>n;
	vector<Point> map1(n+1),map2(n+1);
	auto readMap=[&](vector<Point>& pMap){
		for(int i=1;i<=n;++i) {
			double lo, la;
			cin >> la >> lo;
			pMap[i].latitude = (int(la*scale) + 90*scale) % (180*scale);
			pMap[i].longitude = (int(lo*scale) + 180*scale) % (360*scale);
		}
	};
	readMap(map1);
	readMap(map2);
	//sort according to (longitude, latitude)
	auto cmp=[&](const Point& a, const Point& b){
		return a.longitude<b.longitude || a.longitude==b.longitude && a.latitude<b.latitude;
	};
	sort(map1.begin(),map1.end(),cmp);
	sort(map2.begin(),map2.end(),cmp);
	//compute delta longitude
	map1[1].deltaLongitude-=map1.back().longitude+360*scale;
	map2[1].deltaLongitude-=map2.back().longitude+360*scale;
	for(int i=2;i<=n;++i)
		map1[i].deltaLongitude=map1[i].longitude-map1[i-1].longitude,
		map2[i].deltaLongitude=map2[i].longitude-map2[i-1].longitude;
	//build fail link
	vector<int> fail(n+1,0);//[0] is fake root
	fail[0]=0;
	for(int i=1;i<=n;++i){
		int fPar=fail[i-1];
		auto po=map1[i];
		while( fPar!=0 && !(map1[fPar+1]==po) )
			fPar=fail[fPar];
		fail[i]= fPar!=0 ? fPar+1 : 0 ;
	}
	//use fail link to accelerate comparison
	int met=1;
	//spin the globe 2 rounds, if still no match point found, the 2 maps are different
	for(int i=1;i<=2*n;++i){
		while(!(map2[i]==map1[met]))
			met=fail[met];
		++met;
		if(met>n) {
			cout << "Same";
			return 0;
		}
	}
	cout<<"Different";
	return 0;
}
