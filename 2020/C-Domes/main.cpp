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
	list<Point> polygon={{0,0},{0,dy},{dx,dy},{dx,0}};
	for(int i=0;i<=n-2;++i){
		auto &p1=domeLocs[permutaion[i]], &p2=domeLocs[permutaion[i+1]];
		//取直线一边的polygon为新多边形
		int j=0;
		do{
			double cpj=CrossProduction(p2-p1,polygon[j]-p1),
				cpjp1=CrossProduction(p2-p1,polygon[j+1]-p1);
			if(cpj>0)

		}

	}

}