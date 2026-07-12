MAIN?=main_warmup.c

MAIN_OBJ=${MAIN:.c=.o}

SRC=${MAIN} \
	binary.c \
	linear.c \
	eytzinger.c \
	linear.c \
	pbtree.c \
	stree.c 

OBJ = ${SRC:.c=.o}

CFLAGS=-O0 -g -mbmi -fno-omit-frame-pointer
LDFLAGS=-lm -fno-omit-frame-pointer
LDLIBS=

EXE=bench.exe

all: linear binary pbtree stree

linear: ${MAIN_OBJ} linear.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

binary: ${MAIN_OBJ} binary.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

pbtree: ${MAIN_OBJ} pbtree.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

eytzinger: ${MAIN_OBJ} eytzinger.o
	echo ${CFLAGS}
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

eytzinger_prefetch: ${MAIN_OBJ} eytzinger_prefetch.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

stree: ${MAIN_OBJ} stree.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

stree_plus: ${MAIN_OBJ} stree_plus.o
	${CC} -o ${EXE} $^ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

nowarm_%:
	${MAKE} CFLAGS="$(CFLAGS) -DNO_WARMUP=1" -B $*

b_warm_%:
	MAIN=main_warm_bench.c ${MAKE}  -B $*

perf_%: /tmp/perf_ctl.fifo /tmp/perf_ack.fifo
	${MAKE} CFLAGS="$(CFLAGS) -DPERF_ENABLE=1" -B $*


/tmp/perf_ctl.fifo:
	mkfifo /tmp/perf_ctl.fifo
/tmp/perf_ack.fifo:
	mkfifo /tmp/perf_ack.fifo

clean:
	${RM} *.o *.exe

.PHONY: all linear binary pbtree eytzinger stree clean


