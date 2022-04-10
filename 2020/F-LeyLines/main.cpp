#include "bits/stdc++.h"
using namespace std;

const double pi=M_PI;

struct Vec{
	double x,y;
	Vec operator-(const Vec& b){return Vec{x-b.x,y-b.y};}
	double len(){return hypot(x,y);}
	double theta(){return atan2(y,x);}
};

struct event{
	double angle;
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
			double theta=v.theta(),
			phi= asin((double)t/v.len());//[0,pi/2)
			if(v.len()<=t) {
				events.push_back(event{theta,true,j});
				events.push_back(event{theta+pi,false,j});
			}else{
				events.push_back(event{theta,true,j});
				events.push_back(event{theta+phi,false,j});
				events.push_back(event{theta+pi-phi,true,j});
				events.push_back(event{theta+pi,false,j});
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
		//spin one round ahead to make initial state right, the second round to find max
		findMax(),
		ans=max(ans,findMax());
	}
	cout<<ans<<endl;
	return 0;
}