make clean perf_binary
perf stat -r 5 -e cycles,cache-misses,L1-dcache-load-misses,instructions -o perf_binary.data --control=fifo:/tmp/perf_ctl.fifo,/tmp/perf_ack.fifo -- ./bench.exe

make clean perf_eytzinger
perf stat -r 5 -e cycles,cache-misses,L1-dcache-load-misses,instructions -o perf_eytzinger.data --control=fifo:/tmp/perf_ctl.fifo,/tmp/perf_ack.fifo -- ./bench.exe