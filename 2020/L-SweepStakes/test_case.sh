#!/bin/bash

if [[ $1 == "case" ]];then
  inf="data/$2.in"
  cat $inf |./main > ./data/my_ans
  ./calc.py $2 detail
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
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  cat $inf |./main > ./data/my_ans
  correct=$(cat ./data/$ans_filename)
  ./calc.py "${filename%.*}"
done