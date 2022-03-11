#!/bin/bash

if [[ $1 == "case" ]];then
  correct=$(cat ../icpc2019data/C-checks/$2.ans)
  ans=$(cat ../icpc2019data/C-checks/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo "failed"
  else
    echo "succ"
  fi
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ../icpc2019data/C-checks/my_ans
  exit
fi

input=$(ls ../icpc2019data/C-checks/*.in)
count=0
for inf in $input;do
  if(( $count >= 2 ));then exit; fi
  echo $inf
  cat $inf
  echo "my answer"
  cat $inf |./main
  echo "solution"
  filename=$(basename -- "$inf")
  ans_filename="${filename%.*}"".ans"
  cat ../icpc2019data/C-checks/$ans_filename
  count=$(( $count + 1 ))
done