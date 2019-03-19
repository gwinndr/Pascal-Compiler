# Damon Gwinn
# Makefile for the unit test
CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =


BIN = TreeListUnitTest

all: TreeFifoList.o UnitTest.o
	$(CC) $(CCFLAGS) -o $(BIN) UnitTest.o TreeFifoList.o $(LIBS)

TreeFifoList.o:
	$(CC) $(CCFLAGS) -c TreeFifoList.c

UnitTest.o:
	$(CC) $(CCFLAGS) -c UnitTest.c

clean:
	rm -f *.o $(BIN)
