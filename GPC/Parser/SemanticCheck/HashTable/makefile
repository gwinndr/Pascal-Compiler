# Damon Gwinn
# Makefile for the unit test
CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =


BIN = HashUnitTest

all: List.o HashTable.o UnitTest.o
	$(CC) $(CCFLAGS) -o $(BIN) UnitTest.o HashTable.o List.o $(LIBS)

List.o:
	$(CC) $(CCFLAGS) -c ../../List/List.c

HashTable.o:
	$(CC) $(CCFLAGS) -c HashTable.c

UnitTest.o:
	$(CC) $(CCFLAGS) -c UnitTest.c

clean:
	rm -f *.o $(BIN)
