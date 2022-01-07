#! /bin/bash

function cmp() {
  AC=$(cat test.txt| ./answer);
  AC2=$(cat test.txt| ./answer2);
  AM=$(cat test.txt|python3 solution.py );
  [ $AC == $AM ]
  res=$?
  echo "ac" $AC; echo "ac2" $AC2; echo "am" $AM
  return $res
}

function gen() {
  python3 test_gen.py
  cmp && echo succ || echo fail
}

if [ $1 == "gen_loop" ];then
  for((;;));do
    gen
    if [ $AC != $AM ];then exit;fi
    if [ $AC != $AC2 ];then exit;fi
  done
fi

if [ $1 == "gen" ];then
  gen
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
fi