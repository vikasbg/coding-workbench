CC=gcc
PROJECT=.
INC=${PROJECT}/inc
BUILD=build
CFLAGS= -I$(INC) -g -O2

SRC=${PROJECT}/main.c
SRC+=${PROJECT}/src/src1/add.c
SRC+=${PROJECT}/src/src2/sub.c
SRC+=${PROJECT}/src/src3/mul.c
SRC+=${PROJECT}/src/src4/div.c

OBJ=${BUILD}/main.o
OBJ+=${BUILD}/src/src1/add.o
OBJ+=${BUILD}/src/src2/sub.o
OBJ+=${BUILD}/src/src3/mul.o
OBJ+=${BUILD}/src/src4/div.o

${BUILD}/Daemon: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o $@
	@echo "Daemon Generated"

${BUILD}/%.o:%.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -r ${BUILD}

.PHONY: clean all