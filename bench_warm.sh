if [ "$1" = "" ]; then
    echo Please provide the name of an algorithme as parametter
    echo "usage: ./bench_warm.sh <algo>"
    exit 1
fi

make clean > /dev/null
make b_warm_$1 > /dev/null

for i in $(seq 1 500); do
    taskset -c 3 numactl --membind=0 ./bench.exe
done