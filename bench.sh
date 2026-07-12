#! /bin/sh

benchs="binary eytzinger eytzinger_prefetch stree"

make binary && ./bench.exe 1

for t in $benchs; do
    make clean $t > /dev/null
    taskset -c 3 numactl --membind=0 ./bench.exe
done
