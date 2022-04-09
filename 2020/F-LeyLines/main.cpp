#include "bits/stdc++.h"
using namespace std;

const double pi=M_PI;

struct Vec{
	double x,y;
	Vec operator-(const Vec& b){return Vec{x-b.x,y-b.y};}
	double len(){return hypot(x,y);}
	double theta(){return fmod(atan2(y,x)+2*pi,2*pi);}//atan2 return (-pi,pi], make it (0,2pi]
};

struct event{
	double angle;
	bool enter;//true enter, false leave
};

int main(){
	int n,t;
	cin>>n>>t;
	vector<Vec> points(n);
	for (int i = 0; i < n; ++i)
		cin>>points[i].x>>points[i].y;
	vector<event> onTop,onBottom;
	onTop.reserve(2*n);
	onBottom.reserve(2*n);
	int ans=0;
	for (int i = 0; i <n ; ++i) {
		onTop.clear();
		onBottom.clear();
		int Ps=1;
		for (int j = 0; j <n ; ++j) {
			if(i==j)continue;
			auto v=points[j]-points[i];
			if(v.len()<=t)
				++Ps;
			else{
				auto adjustAngle=[&](double ang)->double{//make angle in [0,2pi] to [0,pi]
					if(ang>pi)
						return ang-pi;
					return ang;
				};
				double theta=v.theta(),
					phi= asin(double(t)/v.len());//(0,pi/2)
				onTop.push_back(event{adjustAngle(theta),true}),
				onTop.push_back(event{adjustAngle(theta+phi),false}),
				onBottom.push_back(event{adjustAngle(theta-phi),true}),
				onBottom.push_back(event{adjustAngle(theta),false});
			}
		}
		auto cmp=[&](const event& a,const event& b){//first key angel, second key the point that is entering comes first
			return a.angle<b.angle || a.angle==b.angle && a.enter==true && b.enter==false;
		};
		sort(onTop.begin(),onTop.end(),cmp);
		sort(onBottom.begin(),onBottom.end(),cmp);
		auto findMax=[&](const vector<event>& events)->int{
			int maxP=Ps;
			for(auto& ev:events)
				if(ev.enter)
					++Ps,
					maxP=max(maxP,Ps);
				else
					--Ps;
			return maxP;
		};
		ans=max(ans, findMax(onTop));
		ans=max(ans, findMax(onBottom));
	}
	cout<<ans;
	return 0;
}