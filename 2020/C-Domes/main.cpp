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
};

int n;
double dx,dy;
vector<Point> domeLocs;
vector<int> permutaion;

Line GetLineFromTwoPoints(const Point&a ,const Point&b){
	double A,B,C;
	A=a.y-b.y;
	B=b.x-a.x;
	C=-(a.x*A+B+a.y);
	return Line{A,B,C};
}
double CrossProduction(const Point& a,const Point& b){return a.x*b.y-a.y*b.x;}
int main(){
	cin>>dx>>dy>>n;
	domeLocs.resize(n);
	permutaion.resize(n);
	for(int i=0;i<n;++i)
		cin>>domeLocs[i].x>>domeLocs[i].y;
	for(int i=0;i<n;++i)
		cin>>permutaion[i];
	//
	vector<Point> polygon={{0,0},{0,dy},{dx,dy},{dx,0}};
	for(int i=0;i<=n-2;++i){
		auto &p1=domeLocs[permutaion[i]], &p2=domeLocs[permutaion[i+1]];
		//求通过per[i],per[i+1]的直线
		Line l= GetLineFromTwoPoints(p1,p2);
		//求polygon和直线的交点, 或者判断polygon是否在直线的一侧
		Point normalVector=Point{p1.y-p2.y,p2.x-p1.x};//p1p2的法向量
		Point p3=p1+normalVector;
		//法向量要指向p1p2的右侧
		if(CrossProduction(p3,p2-p1)<0)
			normalVector.x=-normalVector.x,
			normalVector.y=-normalVector.y;
		//取直线一边的polygon为新多边形
		for(auto& pPoly:polygon)
	}

}