#! /bin/bash

function cmp() {
  AC=$(cat test.txt| ./answer);
  AM=$(cat test.txt| ./main );
  [ $AC == $AM ]
  res=$?
  return $res
}

function gen() {
  python3 test_gen.py
  cmp && echo succ || echo fail
  echo "ac" $AC; echo "am" $AM
}

if [ $1 == "gen_loop" ];then
  for((;;));do
    gen
    if [ $AC != $AM ];then exit;fi
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
  echo "ac" $AC; echo "am" $AM
fi