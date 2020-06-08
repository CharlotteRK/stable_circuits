CC = gcc
FLAGS = -Wall -Wextra -O0 -ggdb -lm

all: circuit logic circuits
clean:
	rm -f *.o

circuits: main.c
	${CC} ${FLAGS} -c main.c -o main.o

circuit: circuit.c
	${CC} ${FLAGS} -c circuit.c -o circuit.o

circuits: circuit.o main.o logic.o
	${CC} ${FLAGS} circuit.o main.o logic.o -o circuits

logic: logic.c
	${CC} ${FLAGS} -c logic.c -o logic.o
