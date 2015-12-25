CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories ${BIN}/sleep ${BIN}/wait ${BIN}/synchro3 ${BIN}/reprise

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/sleep: ${SRC}/sleep.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/wait: ${SRC}/wait.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/synchro3: ${SRC}/synchro3.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/reprise: ${SRC}/reprise.c
	${CC} ${CFLAGS} $^ -o $@

runex1: ${BIN} ${BIN}/wait
	${BIN}/wait

runex2: ${BIN} ${BIN}/sleep
	${BIN}/sleep

runex3: ${BIN} ${BIN}/synchro3
	${BIN}/synchro3

runex4: ${BIN} ${BIN}/reprise
	${BIN}/reprise

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~
