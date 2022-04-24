#!/bin/bash

if [[ $1 == "case" ]];then
  correct=$(cat ./data/$2.ans)
  ans=$(cat ./data/$2.in |./main)
  ./calc.py $correct $ans
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ./data/my_ans
  exit
fi

input=$(ls ./data/*.in)
for inf in $input;do
  echo $inf
  filename=$(basename -- "$inf")
  #if [ "$filename" == "01.in" ];then continue ;fi
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
#  ans=$(cat $inf |./main)
  cat $inf |./main > ./data/my_ans
  correct=$(cat ./data/$ans_filename)
  ./calc.py "${filename%.*}"
done