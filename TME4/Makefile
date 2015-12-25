CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories ${BIN}/voodoo ${BIN}/pending-signals ${BIN}/pending-signals-2 ${BIN}/pending-signals-3 ${BIN}/accountant ${BIN}/spy

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/voodoo: ${SRC}/voodoo.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/pending-signals: ${SRC}/pending-signals.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/pending-signals-2: ${SRC}/pending-signals-2.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/pending-signals-3: ${SRC}/pending-signals-3.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/accountant: ${SRC}/accountant.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/spy: ${SRC}/spy.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@

runex2: ${BIN} ${BIN}/voodoo
	${BIN}/voodoo

runex3: ${BIN} ${BIN}/pending-signals
	${BIN}/pending-signals

runex4: ${BIN} ${BIN}/pending-signals-2
	${BIN}/pending-signals-2

runex5: ${BIN} ${BIN}/pending-signals-3
	${BIN}/pending-signals-3

runex6: ${BIN} ${BIN}/accountant
	${BIN}/accountant

runex7: ${BIN} ${BIN}/spy
	${BIN}/spy

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/* ./affiche

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB} ./affiche
	rm -f ${INC}/*~ ${SRC}/*~ *~
