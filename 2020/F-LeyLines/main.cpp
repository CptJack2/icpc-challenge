#include "bits/stdc++.h"
using namespace std;

const long double pi=M_PI;

struct Vec{
	long double x,y;
	Vec operator-(const Vec& b){return Vec{x-b.x,y-b.y};}
	long double len(){return hypot(x,y);}
	long double theta(){return atan2(y,x);}//atan2 return (-pi,pi], make it (0,2pi]
};

struct event{
	long double angle;
	bool enter;//true enter, false leave
	int index;//for debug
};

int main(){
	int n,t;
	cin>>n>>t;
	vector<Vec> points(n);
	for (int i = 0; i < n; ++i)
		cin>>points[i].x>>points[i].y;
	vector<event> events;//point[i] always on line top
	events.reserve(4*n);
	int ans=0;
	for (int i = 0; i <n ; ++i) {
		events.clear();
		int Ps=1;
		for (int j = 0; j <n ; ++j) {
			if(i==j)continue;
			auto v=points[j]-points[i];
			long double theta=v.theta(),
			phi= asin((long double)t/v.len());//[0,pi/2)
			auto adjust=[&](long double a){return a;};//return fmod(a+4*pi,2*pi);};
			auto inRange=[&](long double ang,long double st, long double ed)->bool{//clockwise range, inclusive
				ang=adjust(ang);
				st=adjust(st);
				ed=adjust(ed);
				if(st<=ed)
					return st<=ang && ed>=ang;
				else
					return st-2*pi<=ang && ed>=ang;
			};
			if(v.len()<=t) {
				auto st=adjust(theta), ed=adjust(theta+pi);
				events.push_back(event{st,true,j});
				events.push_back(event{ed,false,j});
//				if(inRange(0,st,ed))
//					++Ps;
			}else{
				auto st=adjust(theta), ed=adjust(theta+phi);
				events.push_back(event{theta,true,j});
				events.push_back(event{theta+phi,false,j});
//				if(inRange(0,st,ed))
//					++Ps;
				st=adjust(theta+pi-phi), ed=adjust(theta+pi);
				events.push_back(event{theta+pi-phi,true,j});
				events.push_back(event{theta+pi,false,j});
//				if(inRange(0,st,ed))
//					++Ps;
			}
		}
		auto cmp=[&](const event& a,const event& b){//first key angel, second key the point that is entering comes first
			return a.angle<b.angle || a.angle==b.angle && a.enter==true && b.enter==false;
		};
		sort(events.begin(),events.end(),cmp);
		auto findMax=[&]()->int{
			int maxP=Ps;
			for(auto& ev:events)
				if(ev.enter)
					++Ps,
					maxP=max(maxP,Ps);
				else
					--Ps;
			return maxP;
		};
		int fm1=findMax(),
			fm2=findMax();
		ans=max(ans,fm2);
	}
	cout<<ans<<endl;
	return 0;
}