CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
AR=ar

all: directories ${BIN}/autotest

directories: ${BIN} ${LIB}
	cp autotestfic.o ./obj

${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/autotest: ${OBJ}/autotestfic.o ${LIB}/libficindex.a
	${CC} -static ${CFLAGS} -o $@ $< -L${LIB} -lficindex

${OBJ}/ficindex.o: ${SRC}/ficindex.c ${INC}/ficindex.h
	${CC} ${CFLAGS} -c -o $@ $< -I${INC}

${LIB}/libficindex.a: ${OBJ}/ficindex.o
	${AR} rs $@ $<

run-autotest: ${BIN} ${BIN}/autotest
	${BIN}/autotest

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~ tmpfile*
