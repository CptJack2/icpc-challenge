package main

import "fmt"

func min(a,b int)int{if a<b {return a} else {return b} }

func main(){
	var gene,mGene string
	fmt.Scan(&gene)
	buf:=make([]byte,0)
	for _,c:=range gene{
		buf=append(buf,'#', byte(c))
	}
	buf=append(buf,'#')
	mGene=string(buf)
	radius :=make([]int,len(mGene))
	head:=0; tail:=len(mGene)-1
	rBorder:=-1; center:=-1
	for i:=0;i<len(mGene);i++{
		if rBorder>=i{
			radius[i]=min(radius[2*center-i],rBorder-i)
		}else{
			radius[i]=1
		}
		for ;i+radius[i]<len(mGene) && i-radius[i]>=0 && mGene[i+radius[i]]==mGene[i-radius[i]];{
			radius[i]++
		}
		if i+radius[i]-1>rBorder{
			rBorder=i+radius[i]-1
			center=i
		}
		if i%2==0 && radius[i]-1>=i-head{
			head=i
		}
	}
	for i:=len(mGene)-1;i>=head;i-=2 {
		if radius[i]-1 >= tail-i {
			tail-=min(i-head,tail-i)
		}
	}
	ans:=(tail-head)/2
	fmt.Println(ans)
}