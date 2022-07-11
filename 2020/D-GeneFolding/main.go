//TODO this is so much fucking slower than the C++ version, perf it to find out what's happening
package main

import "fmt"

var gene string

func min(a,b int)int{if a<b {return a} else {return b} }

func makeMGene(mGene*[]byte){
	*mGene=make([]byte,0,2*len(gene)+1)
	for _,c:=range gene{
		*mGene=append(*mGene,'#', byte(c))
	}
	*mGene=append(*mGene,'#')
}

func loop(i int,radius[]int,mGene[]byte){
	for ; i+radius[i] < len(mGene) && i-radius[i] >= 0 && mGene[i+radius[i]] == mGene[i-radius[i]]; {
		radius[i]++
	}
}

func if1(i int,radius[]int,rBorder,center *int){
	if i+radius[i]-1 > *rBorder {
		*rBorder = i + radius[i] - 1
		*center = i
	}
}

func if2(i int,radius[]int,head *int){
	if i%2 == 0 && radius[i]-1 >= i-*head {
		*head = i
	}
}

func getRadius(mGene []byte,radius[]int,head,tail *int){
	rBorder := -1
	center := -1
	for i := 0; i < len(mGene); i++ {
		if rBorder >= i {
			radius[i] = min(radius[2*center-i], rBorder-i)
		} else {
			radius[i] = 1
		}
		loop(i,radius,mGene)
		if1(i,radius,&rBorder,&center)
		if2(i,radius,head)
	}
}

func getAns(mGene[]byte,radius[]int,head,tail int,ans*int){
    for i := len(mGene) - 1; i >= head; i -= 2 {
        if radius[i]-1 >= tail-i {
            tail -= min(i-head, tail-i)
        }
    }
    *ans = (tail - head) / 2
}

func main(){
	fmt.Scan(&gene)

	for ii:=0;ii<150;ii++{
		var mGene []byte
		makeMGene(&mGene)

		radius := make([]int, len(mGene))
		head := 0
		tail := len(mGene) - 1
		getRadius(mGene,radius,&head,&tail)


		ans := 0
		getAns(mGene,radius,head,tail,&ans)

		fmt.Println(ans)
		if ii%5==0{fmt.Printf("%d round done\n",ii)}
	}
}