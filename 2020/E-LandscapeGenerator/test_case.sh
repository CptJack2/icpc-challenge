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
  #if [ "$filename" == "01.in" ];then continue ;fi
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  ans=$(cat $inf |./main)
  echo "$ans" > ./data/my_ans
  correct=$(cat ./data/$ans_filename)
  if [ "$ans" != "$correct" ];then
    echo "failed"
#    echo "correct " $correct
#    echo "ans " $ans
    #diff ./data/$ans_filename ./data/my_ans
    #exit
  else
    echo "succ"
  fi
  count=$(( $count + 1 ))
done