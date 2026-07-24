#! /bin/sh

benchs="linear"

make binary $t > /dev/null && ./bench.exe 1

for t in $benchs; do
    make clean $t > /dev/null
    taskset -c 3 numactl --membind=0 ./bench.exe
done
