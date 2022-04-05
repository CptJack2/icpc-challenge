#include "bits/stdc++.h"
using namespace std;

struct Line{//Ax+By+C=0
	double A,B,C;
	bool bigger;//region Ax+By+C>0 or Ax+By+C<0
};

struct Point{
	double x,y;
	Point operator+(const Point& b){return Point{x + b.x,y+b.y};}
	Point operator-(const Point& b){return Point{x -b.x,y-b.y};}
	bool operator==(const Point& b){return x==b.x && y==b.y;}
	bool operator!=(const Point& b){return x!=b.x || y!=b.y;}
};

int n;
double dx,dy;
vector<Point> domeLocs;
vector<int> permutaion;

double CrossProduction(const Point& a,const Point& b){return a.x*b.y-a.y*b.x;}

int main(){
	cin>>dx>>dy>>n;
	domeLocs.resize(n);
	permutaion.resize(n);
	for(int i=0;i<n;++i)
		cin>>domeLocs[i].x>>domeLocs[i].y;
	for(int i=0;i<n;++i)
		cin>>permutaion[i];
	
	vector<Point> polygon={{0,0},{0,dy},{dx,dy},{dx,0}}, polygon2;
	for(int i=0;i<=n-2;++i){
		auto &p1=domeLocs[permutaion[i]-1], &p2=domeLocs[permutaion[i+1]-1];
		polygon2.clear();
		//取直线一边的polygon为新多边形
		for(int j=0;j<polygon.size();++j){
			auto& p3=polygon[j], p4=j!=polygon.size()-1?polygon[j+1]:polygon[0];
			auto cp3=CrossProduction(p2-p1,p3-p1), cp4=CrossProduction(p2-p1,p4-p1);
			//p3点在向量p1p2的右侧
			if(cp3<=0)
				polygon2.push_back(p3);
			//p3和p4中间有新的交点
			if(cp3>0 && cp4<0 || cp3<0 && cp4>0){
				double x1 = p1.x, y1 = p1.y;
				double x2 = p2.x, y2 = p2.y;
				double x3 = p3.x, y3 = p3.y;
				double x4 = p4.x, y4 = p4.y;
				double t = ((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1)) / ((x2 - x1)*(y3 - y4) - (x3 - x4)*(y2 - y1));
				Point pcross{x3 + t*(x4 - x3), y3 + t*(y4 - y3)};
				polygon2.push_back(pcross);
			}
		}
		swap(polygon,polygon2);
		if(polygon.empty())
			break;
	}
	//算面积
	double area=0;
	for(auto it=polygon.begin();it!=polygon.end();++it){
		auto nit=next(it);
		if(nit==polygon.end())
			nit=polygon.begin();
		auto cp=CrossProduction(*it,*nit);
		area+= cp;
	}
	area/=2;
	cout<<setprecision(12)<<abs(area);
	return 0;
}