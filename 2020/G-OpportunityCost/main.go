package main

import (
	"fmt"
	"math"
)

func max(a,b int64)int64{if a>b{return a}else {return b}}

func main(){
	n:=0
	fmt.Scan(&n)
	v:=make([]int,3)
	partialSum:=make([][]int64,n)
	for i:=0;i<n;i++{partialSum[i]=make([]int64,8)}
	maximums:=make([]int64,8)
	var bestCost int64=math.MaxInt64; ind:=-1
	for i:=0;i<n;i++{
		for j:=0;j<3;j++{fmt.Scan(&v[j])}
		for j:=0;j<8;j++{
			var sum int64=0
			if j&0b1!=0{sum+=int64(v[0])}
			if j&0b10!=0{sum+=int64(v[1])}
			if j&0b100!=0{sum+=int64(v[2])}
			maximums[j]=max(maximums[j],sum)
			partialSum[i][j]=sum
		}
	}
	//fmt.Println(maximums)
	//fmt.Println(partialSum)
	//return
	for i:=0;i<n;i++{
		cost:=int64(0)
		for j:=0;j<8;j++{cost=max(cost,maximums[j]-partialSum[i][j])}
		if bestCost>cost{
			bestCost=cost
			ind=i
		}
	}
	fmt.Println(bestCost,ind+1)
}
