#! /bin/sh

benchs="binary eytzinger eytzinger_prefetch stree linear"

make binary && ./bench.exe 1

for t in $benchs; do
    make $t > /dev/null
    taskset -c 12 ./bench.exe
done
