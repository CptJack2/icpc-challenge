#!/bin/bash

input=$(ls ../icpc2019data/A-azulejos/*.in)
count=0
for inf in $input;do
#  if(( $count >= 2 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  ans=$(cat $inf |./main)
  correct=$(cat ../icpc2019data/A-azulejos/$ans_filename)
  if [ "$ans" != "impossible" ];then
    ans="possible"
  fi
  if [ "$correct" != "impossible" ];then
      correct="possible"
  fi
  if [ "$ans" != "$correct" ];then
    echo "failed"
    echo "correct " correct
    echo "ans " $ans
  else
    echo "succ"
  fi
  count=$(( $count + 1 ))
done