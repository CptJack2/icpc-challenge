#include "bits/stdc++.h"
using namespace std;

const long double pi=M_PI;

struct Vec{
	long double x,y;
	Vec operator-(const Vec& b){return Vec{x-b.x,y-b.y};}
	long double len(){return hypot(x,y);}
	long double theta(){return fmod(atan2(y,x)+2*pi,2*pi);}//atan2 return (-pi,pi], make it (0,2pi]
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
				auto adjustAngle=[&](long double ang)->long double{//θ∈(0,2pi], φ∈[0,pi/2), ang=θ±φ∈(-pi/2,5/2pi], adjust them to (-pi/2,pi/2]
					ang=fmod(ang+pi/2+2*pi,2*pi)-pi/2;//(-pi/2,3/2pi]
					if(ang>pi/2)
						return ang-pi;
					return ang;
				};
				long double theta=v.theta(),
					phi= asin((long double)t/v.len());//[0,pi/2)
				if(theta<=pi/2 || theta>3*pi/2)
					onTop.push_back(event{adjustAngle(theta),true,j}),
					onTop.push_back(event{adjustAngle(theta+phi),false,j}),
					onBottom.push_back(event{adjustAngle(theta-phi),true,j}),
					onBottom.push_back(event{adjustAngle(theta),false,j});
				else
					onTop.push_back(event{adjustAngle(theta-phi),true,j}),
					onTop.push_back(event{adjustAngle(theta),false,j}),
					onBottom.push_back(event{adjustAngle(theta),true,j}),
					onBottom.push_back(event{adjustAngle(theta+phi),false,j});
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
		int fmt=findMax(onTop),
			fmb=findMax(onBottom);
		ans=max(ans, fmt);
		ans=max(ans, fmb);
	}
	cout<<ans;
	return 0;
}