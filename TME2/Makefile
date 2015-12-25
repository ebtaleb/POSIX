CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=gnu99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories ${BIN}/ex1it ${BIN}/ex1rec ${BIN}/ex2prime ${BIN}/ex2bis ${BIN}/ex2ter ${BIN}/ex3 ${BIN}/decoder ${BIN}/simple_converter

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/ex1it: ${SRC}/ex1it.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/ex1rec: ${SRC}/ex1rec.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/ex2prime: ${SRC}/ex2prime.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/ex2bis: ${SRC}/ex2bis.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/ex2ter: ${SRC}/ex2ter.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/ex3: ${SRC}/ex3.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/decoder: ${SRC}/decoder.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/simple_converter: ${SRC}/converters.c
	${CC} ${CFLAGS} $^ -o $@ -I${INC}

runex1it:
	${BIN}/ex1it


runex1rec:
	${BIN}/ex1rec

runex2prime:
	${BIN}/ex2prime

runex2bis:
	${BIN}/ex2bis

runex3:
	${BIN}/ex3

runex2ter:
	${BIN}/ex2ter
rundecoder:
	${BIN}/decoder
run_simple_converter:
	${BIN}/simple_converter

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~
