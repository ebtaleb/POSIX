CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
all: directories ${BIN}/ex5  ${BIN}/filefusion  ${BIN}/findword  ${BIN}/liste-rep  ${BIN}/setfilestats

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

${BIN}/liste-rep: ${SRC}/liste-rep.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/setfilestats: ${SRC}/setfilestats.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/filefusion: ${SRC}/filefusion.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/findword: ${SRC}/findword.c
	${CC} ${CFLAGS} $^ -o $@

${BIN}/ex5: ${SRC}/ex5.c
	${CC} ${CFLAGS} $^ -o $@

runliste-rep: ${BIN} ${BIN}/liste-rep
	echo "Contenu du repertoire courant\n"
	${BIN}/liste-rep
	echo "\n"
	echo "Contenu du repertoire src\n"
	${BIN}/liste-rep src

runsetfilestats: ${BIN} ${BIN}/setfilestats
	touch ${BIN}/test
	touch ${BIN}/test2
	${BIN}/setfilestats E ${BIN}/test
	${BIN}/setfilestats R ${BIN}/test2 ${BIN}/test3

runfilefusion: ${BIN} ${BIN}/filefusion
	cat a
	cat b
	${BIN}/filefusion a b c
	cat c
	rm c

runfindword: ${BIN} ${BIN}/findword
	${BIN}/findword sdf derp

runex5: ${BIN} ${BIN}/ex5
	${BIN}/ex5 nope
	cat nope
	rm nope

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/* ./affiche

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB} ./affiche
	rm -f ${INC}/*~ ${SRC}/*~ *~
