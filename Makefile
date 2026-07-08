SRC=main.c \
	binary.c \
	linear.c \
	eytzinger.c \
	linear.c \
	pbtree.c \
	stree.c

OBJ = ${SRC:.c=.o}

CFLAGS=-O0 -g -fsanitize=address -mbmi
LDFLAGS=-lm
LDLIBS=-fsanitize=address

all: linear binary pbtree stree

linear: main.o linear.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

binary: main.o binary.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

pbtree: main.o pbtree.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

eytzinger: main.o eytzinger.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

eytzinger_prefetch: main.o eytzinger_prefetch.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

stree: main.o stree.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}

stree_plus: main.o stree_plus.o
	${CC} -o bench.exe $^ ${CLFAGS} ${LDFLAGS} ${LDLIBS}



clean:
	${RM} *.o bench.exe

.PHONY: all linear binary pbtree eytzinger stree clean


