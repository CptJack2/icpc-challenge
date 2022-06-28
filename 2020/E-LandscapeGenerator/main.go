package main

import "fmt"

func main(){
	var n,k int
	fmt.Scanln(&n,&k)
	diff:=make([]int,n+2); diffSlope:=make([]int,n+1)
	for i:=0;i<k;i++{
		var(
			op string
			x1,x2 int
		)
		fmt.Scanln(&op,&x1,&x2)
		addSlope:=func(sl int){
			if x1==x2-1{return}
			mid:=(x1+x2)/2
			diffSlope[x1]+=sl
			if (x2-x1)%2==0{
				diffSlope[mid]-=2*sl
			}else{
				diffSlope[mid]-=sl
				diffSlope[mid+1]-=sl
			}
			diffSlope[x2]+=sl
		}
		addHeight:=func(h int){
			diff[x1]+=h
			diff[x2+1]-=h
		}
		switch op{
		case "H":
			addHeight(1)
			addSlope(1)
		case "V":
			addHeight(-1)
			addSlope(-1)
		case "R":
			addHeight(1)
		case "D":
			addHeight(-1)
		}
	}
	ret:=make([]int64,n+1)
	slope:=diffSlope[1]
	for i:=2;i<=n;i++{
		ret[i]=ret[i-1]+int64(slope)
		slope+=diffSlope[i]
	}
	dif:=diff[1]
	for i:=1;i<=n;i++{
		ret[i]+=int64(dif)
		fmt.Println(ret[i])
		dif+=diff[i+1]
	}
}
