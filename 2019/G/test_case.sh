#!/bin/bash

if [[ $1 == "case" ]];then
  correct=$(cat ../icpc2019data/G-firstofhername/$2.ans)
  ans=$(cat ../icpc2019data/G-firstofhername/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo "failed"
  else
    echo "succ"
  fi
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ../icpc2019data/G-firstofhername/my_ans
  exit
fi

input=$(ls ../icpc2019data/G-firstofhername/*.in)
count=0
for inf in $input;do
#  if(( $count >= 2 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  ans=$(cat $inf |./main)
  echo "$ans" > ../icpc2019data/G-firstofhername/my_ans
  correct=$(cat ../icpc2019data/G-firstofhername/$ans_filename)
  if [ "$ans" != "$correct" ];then
    echo "failed"
#    echo "correct " $correct
#    echo "ans " $ans
    diff ../icpc2019data/G-firstofhername/$ans_filename ../icpc2019data/G-firstofhername/my_ans
    exit
  else
    echo "succ"
  fi
  count=$(( $count + 1 ))
done