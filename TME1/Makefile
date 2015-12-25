CC=gcc -g -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
AR=ar


all: directories runstack


directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}


${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC}

${OBJ}/stack_list.o: ${INC}/stack.h ${SRC}/stack_list.c
	${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}

${OBJ}/fifo_list.o: ${INC}/fifo.h ${SRC}/fifo_list.c
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}

${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}

${OBJ}/list_impl.o: ${INC}/stack.h ${SRC}/list_impl.c
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
	${CC} -DSTACK_SIZE=32 -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}

${BIN}/stack_array_test: ${LIB}/libstack.a ${OBJ}/stack_test.o 
	${CC} -static -o ${BIN}/stack_array_test ${OBJ}/stack_test.o -L${LIB} -lstack 

${BIN}/stack_list_test: ${LIB}/libstack2.a ${OBJ}/stack_test.o 
	${CC} -static -o ${BIN}/stack_list_test ${OBJ}/stack_test.o -L${LIB} -lstack2

${BIN}/fifo_test: ${LIB}/libfifo.a ${OBJ}/fifo_test.o 
	${CC} -static -o ${BIN}/fifo_test ${OBJ}/fifo_test.o -L${LIB} -lfifo

${LIB}/libstack.a: ${OBJ}/stack_array.o
	${AR} rs $@ $<

${LIB}/libstack2.a: ${OBJ}/stack_list.o ${OBJ}/list_impl.o
	${AR} rs $@ $^

${LIB}/libfifo.a: ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	${AR} rs $@ $^

runstack: ${BIN}/stack_array_test ${BIN}/stack_list_test ${BIN}/fifo_test
	echo "Testing Stack Array"
	${BIN}/stack_array_test
	echo "Testing Stack List"
	${BIN}/stack_list_test
	echo "Testing FIFO List"
	${BIN}/fifo_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~

