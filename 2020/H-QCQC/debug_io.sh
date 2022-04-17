#! /bin/bash

function reader() {
  rm input.txt
  touch input.txt
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
    echo $newl
  done
}

function writer() {
  rm output.txt
  touch output.txt

  while [ true ]; do
    read out
    if [[ $out != "" ]];then
      echo $out>>output.txt
    fi
  done
}

reader &
writer
wait