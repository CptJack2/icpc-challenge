#!/bin/bash

if [[ -z "$1" && $1 == "case" ]];then
  correct=$(cat ../icpc2019data/I-karel/$2.ans)
  ans=$(cat ../icpc2019data/I-karel/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo "failed"
  else
    echo "succ"
  fi
  exit
fi

input=$(ls ../icpc2019data/I-karel/*.in)
count=0
for inf in $input;do
#  if(( $count >= 2 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  ans=$(cat $inf |./main)
  correct=$(cat ../icpc2019data/I-karel/$ans_filename)
  if [ "$ans" != "$correct" ];then
    echo "failed"
#    echo "correct " $correct
#    echo "ans " $ans
  else
    echo "succ"
  fi
  count=$(( $count + 1 ))
done