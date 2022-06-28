package main

import "fmt"

var (
	graph []map[int]int
	allSpeed []int
	n,c int
)

func dfs(node,parent int)(int,[]int){
	signCost:=len(graph[node])*c; if(len(graph[node])<=1){signCost=0}
	costs:=make([]int,len(allSpeed))
	maxSpeed:=0; parS:=graph[node][parent]
	for ch,s:= range graph[node]{
		if maxSpeed< s {maxSpeed=s}
		if ch==parent{continue}
		chBC,chC:=dfs(ch,node)
		signCost+=chBC
		for i:=0;i< len(allSpeed);i++{ costs[i]+=chC[i]}
	}
	bestCost:=signCost
	for i:=0;i<len(allSpeed);i++{
		p:=allSpeed[i]-parS; if parent==0{ p=0}
		if allSpeed[i]<maxSpeed{
			costs[i]=signCost+p
		}else{
			costs[i]+=p
			if bestCost>costs[i] {bestCost=costs[i]}
			if costs[i]>signCost+p {costs[i]=signCost+p}
		}
	}
	return bestCost,costs
}

func main(){
	fmt.Scan(&n,&c)
	graph=make([]map[int]int,n+1)
	uniqueSpeeds:=make(map[int]bool)
	allSpeed=make([]int,0)
	for i:=0;i<n-1;i++{
		var u,v,s int
		fmt.Scan(&u,&v,&s)
		if graph[u]==nil {graph[u]=make(map[int]int)}
		graph[u][v]=s
		if graph[v]==nil {graph[v]=make(map[int]int)}
		graph[v][u]=s
		uniqueSpeeds[s]=true
	}
	for s,_:=range uniqueSpeeds{
		allSpeed=append(allSpeed,s)
	}
	bc,_:=dfs(1,0)
	fmt.Println(bc)
}