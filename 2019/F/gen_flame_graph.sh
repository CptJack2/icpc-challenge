#!/bin/bash

perf record -F 99 -p $1 -g -- sleep 60
perf script -i perf.data &> perf.unfold
../../../FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded
../../../FlameGraph/flamegraph.pl perf.folded > perf.svg