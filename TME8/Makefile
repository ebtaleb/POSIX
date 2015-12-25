CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=c99
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
AR=ar

all: directories ${BIN}/single_converter ${BIN}/conversion_server ${BIN}/conversion_client ${BIN}/multi_converter

directories: ${BIN} ${LIB} ${OBJ}

${OBJ}:
	mkdir ${OBJ}

${BIN}:
	mkdir ${BIN}

${LIB}:
	mkdir ${LIB}

${BIN}/single_converter: ${SRC}/converters.c
	${CC} ${CFLAGS} $^ -o $@ -I${INC}

${BIN}/conversion_server: ${SRC}/conversion_server.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@ -I${INC}

${BIN}/conversion_client: ${SRC}/conversion_client.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@ -I${INC}

${BIN}/multi_converter: ${SRC}/multi_converter.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@ -I${INC}

run-ex1: ${BIN} ${BIN}/simple_converter
	${BIN}/simple_converter CNY "100.0"

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${BIN} ${LIB} ${OBJ}
	rm -f ${INC}/*~ ${SRC}/*~ *~
