CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
AR=ar

all: directories ${BIN}/test1_myqueue ${BIN}/test2_myqueue ${BIN}/test1 ${BIN}/test2

directories: ${BIN} ${LIB} ${OBJ}

${OBJ}:
	mkdir ${OBJ}

${BIN}:
	mkdir ${BIN}

${LIB}:
	mkdir ${LIB}

${OBJ}/myqueue.o: ${SRC}/myqueue.c
	${CC} ${CFLAGS} -c -o $@ $< -I${INC}

${LIB}/libmyqueue.a: ${OBJ}/myqueue.o
	${AR} rs $@ $<

${BIN}/test1: ${SRC}/test1.c
	${CC} ${CFLAGS} $^ -o $@ -lpthread -lrt

${BIN}/test2: ${SRC}/test2.c
	${CC} ${CFLAGS} $^ -o $@ -lpthread -lrt

${BIN}/test1_myqueue: ${SRC}/test1_myqueue.c ${LIB}/libmyqueue.a
	${CC} -static ${CFLAGS} $^ -o $@ -I${INC} -L${LIB} -lmyqueue -lpthread -lrt

${BIN}/test2_myqueue: ${SRC}/test2_myqueue.c ${LIB}/libmyqueue.a
	${CC} -static ${CFLAGS} $^ -o $@ -I${INC} -L${LIB} -lmyqueue -lpthread -lrt

run-test1-norm: ${BIN} ${BIN}/test1
	${BIN}/test1

run-test2-norm: ${BIN} ${BIN}/test2
	${BIN}/test2

run-test1: ${BIN} ${BIN}/test1_myqueue
	${BIN}/test1_myqueue

run-test2: ${BIN} ${BIN}/test2_myqueue
	${BIN}/test2_myqueue

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
	rm /dev/shm/*

cleanall:
	rm -rf ${BIN} ${LIB} ${OBJ}
	rm -f ${INC}/*~ ${SRC}/*~ *~
	rm /dev/shm/*
