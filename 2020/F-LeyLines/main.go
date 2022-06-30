package main

import (
	"fmt"
	"math"
	"sort"
)

type Vec struct{
	x,y float64
}

func (v*Vec)minus(v2*Vec)Vec{return Vec{v.x-v2.x,v.y-v2.y}}

func (v*Vec)len()float64{return math.Sqrt(v.x*v.x+v.y*v.y)}

func(v*Vec)theta()float64{return math.Atan2(v.y,v.x)}

type event struct{
	angle float64
	enter bool
	index int
}

func main(){
	var n,t int
	fmt.Scan(&n,&t)
	points:=make([]Vec,n)
	for i:=0;i<n;i++{fmt.Scan(&points[i].x,&points[i].y)}
	events:=make([]event,0,4*n)
	ans:=0
	for i:=0;i<n;i++{
		events=events[:0]//keeps the capacity
		Ps:=1
		for j:=0;j<n;j++{
			if i==j{continue}
			v:=points[j].minus(&points[i])
			theta:=v.theta()
			phi:=math.Asin(float64(t)/v.len())
			if v.len()<=float64(t){
				events=append(events,event{theta,true,j})
				events=append(events,event{theta+math.Pi,false,j})
			}else{
				events=append(events,event{theta,true,j})
				events=append(events,event{theta+phi,false,j})
				events=append(events,event{theta+math.Pi-phi,true,j})
				events=append(events,event{theta+math.Pi,false,j})
			}
		}
		sort.Slice(events, func(i, j int) bool {
			return  events[i].angle<events[j].angle || events[i].angle==events[j].angle && events[i].enter==true && events[j].enter==false
		})
		findMax:=func()int{
			maxP:=Ps
			for _,ev:=range events{
				if ev.enter{
					Ps++
					if Ps>maxP{maxP=Ps}
				}else{Ps--}
			}
			return maxP
		}
		findMax()
		f2:=findMax()
		if f2>ans{ans=f2}
	}
	fmt.Println(ans)
}
