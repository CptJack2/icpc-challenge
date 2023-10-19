#!/bin/bash
if [[ $1 == "case" ]];then
  correct=$(cat ./data/$2.ans)
  ans=$(cat ./data/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo "failed"
    echo "correct " $correct
    echo "ans " $ans
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
  ./verify.py "${filename%.*}"
done