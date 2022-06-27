package main

import (
	"fmt"
	"math"
)

type Pnt struct{
	x,y float64
}

func (p1 *Pnt)plus(p2 *Pnt) Pnt{
	return Pnt{p1.x+p2.x,p1.y+p2.y}
}

func (p1* Pnt)minus(p2*Pnt)Pnt{return Pnt{p1.x-p2.x,p1.y-p2.y}}

func (p1*Pnt)equal(p2*Pnt)bool{return p1.x==p2.x && p1.y==p2.y}

func(p1*Pnt)notEqual(p2*Pnt)bool{return p1.x!=p2.x || p1.y!=p2.y}

var (
	n int
	dx,dy float64
	domeLocs []Pnt
	permutation []int
)

func CrossProduction(a Pnt,b Pnt)float64{return a.x*b.y-a.y*b.x}

func main(){
	fmt.Scanf("%f %f %d",&dx,&dy,&n)
	domeLocs=make([]Pnt,n)
	permutation=make([]int,n)
	for i:=0;i<n;i++{
		var p Pnt
		fmt.Scanf("%f %f",&p.x,&p.y)
		domeLocs=append(domeLocs,p)
	}
	for i:=0;i<n;i++{
		var ii int
		fmt.Scanf("%d",&ii)
		permutation=append(permutation,i)
	}

	polygon:=[]Pnt{{0,0},{0,dy},{dx,dy},{dx,0}}
	polygon2:=make([]Pnt,0)
	for i:=0;i<=n-2;i++{
		for j:=i+1;j<=n-1;j++{
			p1:=domeLocs[permutation[i]-1]; p2:=domeLocs[permutation[j]-1]
			polygon2=make([]Pnt,0)
			for k:=0;k<len(polygon2);k++{
				p3:=polygon[k]; p4:=polygon[k+1]; if k==len(polygon)-1{p4=polygon[0]}
				cp3:=CrossProduction(p2.minus(&p1),p3.minus(&p1)); cp4:=CrossProduction(p2.minus(&p1),p4.minus(&p1))
				if cp3<=0{polygon2=append(polygon2,p3)}
				if cp3>0&& cp4<0 || cp3<0 && cp4>0{
					x1:=p1.x; y1:=p1.y
					x2:=p2.x; y2:=p2.y
					x3:=p3.x; y3:=p3.y
					x4:=p4.x; y4:=p4.y
					t:=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/ ((x2-x1)*(y3-y4)-(x3-x4)*(y2-y1))
					pcross:=Pnt{x3+t*(x4-x3),y3+t*(y4-y3)}
					polygon2=append(polygon2,pcross)
				}
			}
			polygon=polygon2
			if len(polygon)==0{break}
		}
	}

	var area float64=0;
	for i,po:=range polygon{
		j:=i+1
		if i+1==len(polygon)-1{j=0}
		cp:=CrossProduction(po,polygon[j])
		area+=cp
	}
	area/=2
	fmt.Printf("%.12f",math.Abs(area))
}
