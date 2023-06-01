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
g++ -g -std=c++17 -omain main.cpp
g++ -g -std=c++17 -omain2 main2.cpp
for inf in $input;do
  if(( $count >= 1 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  #if [ "$filename" == "01.in" ];then continue ;fi
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  echo answer
  cat $inf |./main
  echo
#  echo "$ans" > ./data/my_ans
  echo correct
  cat $inf |./main2
  echo
  count=$(( $count + 1 ))
done