package main

import(
	"fmt"
)

var r,c int64

func abs(x int64) int64 {
	if x < 0 {
		return -x
	}
	return x
}

func min(a,b int64)int64{
	if a<b{return a}else{return b}
}

func main(){
	fmt.Scanf("%d %d",&r,&c)
	type pll struct{ r,c int64}
	type ansStru struct{
		rc pll
		iterationTimes int
	}
	stablePos:=make([]ansStru,c)

	for p:=int64(0);p<c;p++{
		start:=pll{0,0}; end:=pll{r-1,c-1}
		iterationTime:=0
		pthStart:=pll{r*p/c,r*p%c};
		for ;true;{
			rcAdd:=func(rc pll, v int64)pll{
				ret:=rc
				ret.r+=v/c
				v%=c
				ret.c+=v
				if ret.c>=c{
					ret.c%=c
					ret.r++
				}
				return ret
			}
			oS:=start; oE:=end
			start=rcAdd(pthStart,start.r)
			end=rcAdd(pthStart,end.r)
			if oS==start && oE==end{break}
			iterationTime++
		}
		if start==end {stablePos[p].rc=start} else {stablePos[p].rc=pll{-1,-1}}
		stablePos[p].iterationTimes=iterationTime
	}
	var ans int; minDist:=int64(99999999)

	calDist:=func(rc pll)int64{
		mr:=r/2; mc:=c/2
		minr:=abs(rc.r-mr); minc:=abs(rc.c-mc)
		if r%2==0{
			minr=min(minr,abs(rc.r-mr+1))
		}
		if c%2==0{
			minc=min(minc,abs(rc.c-mc+1))
		}
		return minr+minc
	}
	for i:=0;i<len(stablePos);i++{
		pi:=stablePos[i].rc
		dist:=calDist(pi)
		if (pi.r!=-1 && pi.c!=-1) && dist<minDist{
			minDist=dist
			ans=i
		}
	}
	fmt.Println(ans+1,stablePos[ans].rc.r+1,stablePos[ans].rc.c+1,
		stablePos[ans].iterationTimes)
}













