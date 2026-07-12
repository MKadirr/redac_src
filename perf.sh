make clean perf_binary
taskset -c 2,3 numactl --membind=0 perf stat -r 20 --delay=-1 -e cycles,cache-misses,L1-dcache-load-misses,dTLB-load-misses,instructions -o perf_binary.data --control=fifo:/tmp/perf_ctl.fifo,/tmp/perf_ack.fifo -- ./bench.exe

make clean perf_eytzinger
taskset -c 2,3 numactl --membind=0 perf stat -r 20 --delay=-1 -e cycles,cache-misses,L1-dcache-load-misses,dTLB-load-misses,instructions -o perf_eytzinger.data --control=fifo:/tmp/perf_ctl.fifo,/tmp/perf_ack.fifo -- ./bench.exe