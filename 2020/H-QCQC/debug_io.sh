#! /bin/bash

rm input.txt
touch input.txt
rm output.txt
touch output.txt

lnum=$(wc -l input.txt|awk '{print $1}')

while [ true ]; do
  while [ true ]; do
    newlnum=$(wc -l input.txt|awk '{print $1}')
    if [[ $newlnum != $lnum ]];then
      lnum=$newlnum;
      break;
    fi
  done
  newl=$(tail -n1 input.txt)
  echo $newl >> output.txt
done