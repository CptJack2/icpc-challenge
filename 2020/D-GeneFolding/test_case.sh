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

startTime=`date +%Y%m%d-%H:%M:%S`
startTime_s=`date +%s``date "+%N"`

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
    diff ./data/$ans_filename ./data/my_ans
    exit
  else
    echo "succ"
  fi
  count=$(( $count + 1 ))
done

endTime=`date +%Y%m%d-%H:%M:%S`
endTime_s=`date +%s``date "+%N"`

sumTime=$[ ($endTime_s - $startTime_s)/1000000 ]

echo "$startTime ---> $endTime" "Total:$sumTime micro seconds"