package main

import "fmt"

type Component []int

func main(){
	batch:=0
	fmt.Scan(&batch)
	for i:=0;i<batch;i++{
		n:=0
		fmt.Scan(&n)
		active:=make([]Component,0); leftOver:=make([]Component,0)
		paired:=make([][2]Component,0)
		for j:=0;j<=n;j++{

		}
	}
}
