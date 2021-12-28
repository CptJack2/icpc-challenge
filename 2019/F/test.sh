#! /bin/bash

function cmp() {
  AC=$(cat test.txt| ./answer);
  AM=$(cat test.txt|python3 solution.py );
  [ $AC == $AM ]
  res=$?
  return $res
}

if [ $1 == "gen" ];then
  python3 test_gen.py
  cmp && echo succ || echo fail
  echo "ac" $AC; echo "am" $AM
fi

if [ $1 == "roll" ];then
  for((;;));do
    python roll.py;
    cmp
    if [ $? != 0 ];then
      echo "found error";
      break;
    fi
  done
fi

if [ $1 == "cmp" ];then
  cmp && echo succ || echo fail
  echo "ac" $AC; echo "am" $AM
fi