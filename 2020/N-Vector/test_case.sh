#!/bin/bash

if [[ $1 == "case" ]];then
  inf="data/$2.in"
  cat $inf |./main > ./data/my_ans
  ./validate $inf "data/$2.ans" . < data/my_ans
  if [[ $? -eq 42 ]]; then
    echo succ
  else
    echo fail
  fi
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ./data/my_ans
  exit
fi

input=$(ls ./data/*.in)
for inf in $input;do
  echo $inf
  #if [[ $(ls -s $inf|awk '{print $1}') -gt 1000 ]];then continue; fi
  #if [[ $inf == "./data/000045_NM_MAX_Q_MAX_S_MAX_P_MAX_T_MAX.in" ]];then continue; fi
  filename=$(basename -- "$inf")
  ans_filename="${filename%.*}"".ans"
  cat $inf |./main > ./data/my_ans
  correct=$(cat ./data/$ans_filename)
  ./validate $inf data/$ans_filename . < data/my_ans
  if [[ $? -eq 42 ]]; then
    echo succ
  else
    echo fail
  fi
done