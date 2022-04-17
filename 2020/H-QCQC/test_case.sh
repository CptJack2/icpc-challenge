#!/bin/bash

if [[ $1 == "case" ]];then
#  str=$(python testing_tool.py --verbose ./data/$2.in python onufry.py |grep "<")
  for s in $(python testing_tool.py --verbose ./data/$2.in python onufry.py |grep "<");do
    if [[ $s != "<" ]];then
      echo $s >> $2.in2
    fi
  done
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ./data/my_ans
  exit
fi

input=$(ls ./data/*.in)
count=0
for inf in $input;do
#  if(( $count >= 2 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  succ=$(python testing_tool.py $inf ./main |grep "Wrong answer")
  if [ "$succ" == "" ];then
    echo "succ"
  else
    echo "failed"
    exit
  fi
  count=$(( $count + 1 ))
done