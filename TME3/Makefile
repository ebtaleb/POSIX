CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=gnu99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories ${BIN}/ex1 ${BIN}/fork-exec ${BIN}/ex3 ${BIN}/ex4 ${BIN}/spy

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/ex1: ${SRC}/ex1.c
	${CC} -DN_FILS=11 ${CFLAGS} $^ -o $@

${BIN}/fork-exec: ${SRC}/fork-exec.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/ex3: ${SRC}/ex3.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/ex4: ${SRC}/ex4.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/spy: ${SRC}/spy.c
	${CC} ${CFLAGS} $^ -o $@

runex1: ${BIN} ${BIN}/ex1
	${BIN}/ex1

runfork-exec: ${BIN} ${BIN}/fork-exec
	${BIN}/fork-exec

runex3: ${BIN} ${BIN}/ex3
	${BIN}/ex3 $(SRC)/affiche1.c $(SRC)/affiche2.c $(SRC)/affiche3.c $(SRC)/affiche4.c $(SRC)/affiche-main.c
	./affiche

runex4: ${BIN} ${BIN}/ex4
	${BIN}/ex4

runspy: ${BIN} ${BIN}/spy
	${BIN}/spy

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/* ./affiche

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB} ./affiche
	rm -f ${INC}/*~ ${SRC}/*~ *~
