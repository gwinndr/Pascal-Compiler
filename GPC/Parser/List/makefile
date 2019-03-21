# Damon Gwinn
# Makefile for the unit test
CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =


BIN = ListUnitTest

all: List.o UnitTest.o
	$(CC) $(CCFLAGS) -o $(BIN) UnitTest.o List.o $(LIBS)

List.o:
	$(CC) $(CCFLAGS) -c List.c

UnitTest.o:
	$(CC) $(CCFLAGS) -c UnitTest.c

clean:
	rm -f *.o $(BIN)
