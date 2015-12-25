CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -pedantic -std=gnu99
LIBFLAGS=-lrt
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
AR=ar

all: directories ${BIN}/capitalize ${BIN}/conversion_client ${BIN}/multi_converter

directories: ${BIN} ${LIB} ${OBJ}

${OBJ}:
	mkdir ${OBJ}

${BIN}:
	mkdir ${BIN}

${LIB}:
	mkdir ${LIB}

${BIN}/capitalize: ${SRC}/capitalize.c
	${CC} ${CFLAGS} $^ -o $@ $(LIBFLAGS)

${BIN}/conversion_client: ${SRC}/conversion_client.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@ -I${INC} $(LIBFLAGS)

${BIN}/multi_converter: ${SRC}/multi_converter.c
	${CC} -D_POSIX_C_SOURCE ${CFLAGS} $^ -o $@ -I${INC} $(LIBFLAGS)

run-ex1: ${BIN} ${BIN}/capitalize
	${BIN}/capitalize

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${BIN} ${LIB} ${OBJ}
	rm -f ${INC}/*~ ${SRC}/*~ *~
