# Damon Gwinn
# Makefile for the unit test
CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =


BIN = TreeListUnitTest

all: TreeLinkedList.o UnitTest.o
	$(CC) $(CCFLAGS) -o $(BIN) UnitTest.o TreeLinkedList.o $(LIBS)

TreeLinkedList.o:
	$(CC) $(CCFLAGS) -c TreeLinkedList.c

UnitTest.o:
	$(CC) $(CCFLAGS) -c UnitTest.c

clean:
	rm -f *.o $(BIN)
