#include <bits/stdc++.h>
using namespace std;

struct Point{
	int latitude;//(-90,90)
	int longitude;//(0,360)
	int deltaLongitude;//difference of longitude between this point and previous
};

bool operator==(const Point& a, const Point& b){
	return a.deltaLongitude==b.longitude && a.latitude==b.latitude;
}

constexpr int scale=10000;//at most four digit after decimal point, transfer float to int by multiplying

int main(){
	//read in data
	int n;
	cin>>n;
	vector<Point> map1(n),map2(n);
	auto readMap=[&](vector<Point>& pMap){
		for(int i=0;i<n;++i) {
			double lo, la;
			cin >> la >> lo;
			pMap[i].latitude = int(la*scale);//(int(la*scale) + 90*scale) % (180*scale);
			pMap[i].longitude = int(lo*scale);//(int(lo*scale) + 180*scale);// % (360*scale);
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
	map1[0].deltaLongitude=map1[0].longitude-map1.back().longitude+360*scale;
	map2[0].deltaLongitude=map2[0].longitude-map2.back().longitude+360*scale;
	for(int i=1;i<=n;++i)
		map1[i].deltaLongitude=map1[i].longitude-map1[i-1].longitude,
		map2[i].deltaLongitude=map2[i].longitude-map2[i-1].longitude;
	//build fail link
	vector<int> fail(n,0);//use -1 as a fake root
	fail[0]=-1;
	for(int i=1;i<n;++i){
		int fPar=fail[i-1];
		const auto& pnt=map1[i];
		while( fPar!=-1 && !(map1[fPar+1] == pnt) )
			fPar=fail[fPar];
		fail[i]= fPar!=-1 ? fPar+1 : -1 ;
	}
	auto getFail=[&](int i){return i!=-1 ? fail[i] : -1 ;};
	//use fail link to accelerate comparison
	int met=0;
	//spin the globe 2 rounds, if still no match point found, the 2 maps are different
	for(int i=0;i<=2*n-2;++i){
		while(!(map2[i%n]==map1[met]) && getFail(met-1)!=-1)
			met=getFail(met-1)+1;
		if(met!=-1)
			++met;
		else
			met=0;
		if(met>=n) {
			cout << "Same";
			return 0;
		}
	}
	cout<<"Different";
	return 0;
}
