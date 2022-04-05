/*计算几何题,如果要dome di出现在dj左边,那么只有向量didj右侧的区域是可行的.
 * 初始设区域为整个广场,对每一对didj直线分别更新区域(一定是一个凸多边形),最后算面积*/
#include "bits/stdc++.h"
using namespace std;

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
	//read in data
	cin>>dx>>dy>>n;
	domeLocs.resize(n);
	permutaion.resize(n);
	for(int i=0;i<n;++i)
		cin>>domeLocs[i].x>>domeLocs[i].y;
	for(int i=0;i<n;++i)
		cin>>permutaion[i];
	//update available zone
	vector<Point> polygon={{0,0},{0,dy},{dx,dy},{dx,0}}, polygon2;
	for(int i=0;i<=n-2;++i)
	for(int j= i + 1; j <= n - 1; ++j){//这里排列中每一对i<j的didj都要处理一遍,否则可能出现i在j左边,j在k左边,但i不在k左边的情况
		auto &p1=domeLocs[permutaion[i]-1], &p2=domeLocs[permutaion[j] - 1];
		polygon2.clear();
		//取直线一边的polygon为新多边形
		for(int k=0; k < polygon.size(); ++k){
			auto& p3=polygon[k], p4= k != polygon.size() - 1 ? polygon[k + 1] : polygon[0];
			//通过叉积判断点和直线p1p2的关系
			auto cp3=CrossProduction(p2-p1,p3-p1), cp4=CrossProduction(p2-p1,p4-p1);
			//p3点在向量p1p2的右侧,在新可行域中保留
			if(cp3<=0)
				polygon2.push_back(p3);
			//过p1p2的直线和过p3p4的线段中间有交点,将其加入新可行域
			if(cp3>0 && cp4<0 || cp3<0 && cp4>0){
				//计算交点
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
	//通过多边形所有相邻两点的叉积算面积
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