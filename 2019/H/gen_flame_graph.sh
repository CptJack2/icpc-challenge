#!/bin/bash

pid=$(ps aux|grep main|grep -v grep|awk '{print $2}')
perf record -F 99 -p $pid -g -- sleep 60
perf script -i perf.data &> perf.unfold
../../../FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded
../../../FlameGraph/flamegraph.pl perf.folded > perf.svg