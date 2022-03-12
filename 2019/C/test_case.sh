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
  if(( $count <= 1 ));then
    count=$(( $count + 1 ))
    continue
  fi
  #if(( $count >= 4 ));then exit; fi
  echo $inf
  cat $inf
  echo "my answer"
  cat $inf |./main
  echo "solution"
  filename=$(basename -- "$inf")
  ans_filename="${filename%.*}"".ans"
  cat ../icpc2019data/C-checks/$ans_filename

  ans=$(cat $inf |./main)
  correct=$(cat ../icpc2019data/C-checks/$ans_filename)
  if [ "$ans" != "$correct" ];then
    echo "failed"
    echo "$ans" > ../icpc2019data/C-checks/my_ans
    diff ../icpc2019data/C-checks/$ans_filename ../icpc2019data/C-checks/my_ans
    exit
  else
    echo "succ"
  fi

  count=$(( $count + 1 ))
done