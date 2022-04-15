#!/bin/bash

if [[ $1 == "case" ]];then
  correct=$(cat ./data/$2.ans)
  ans=$(cat ./data/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo "failed"
  else
    echo "succ"
  fi
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
  succ=$(python testing_tool.py $inf python onufry.py |grep "Accepted")
  if [ "$succ" != "" ];then
    echo "succ"
  else
    echo "failed"
  fi
  count=$(( $count + 1 ))
done