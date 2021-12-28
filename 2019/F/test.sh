#! /bin/bash

python3 test_gen.py && AC=$(cat test.txt| ./answer);AM=$(cat test.txt|python3 solution.py ); [ $AC -eq $AM ] && echo "succ" || echo "fail"; echo "ac" $AC; echo "am" $AM