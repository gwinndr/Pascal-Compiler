# Damon Gwinn
# Makefile for the unit test
CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =


BIN = SymtabUnitTest

all: List.o HashTable.o SymTab.o UnitTest.o
	$(CC) $(CCFLAGS) -o $(BIN) UnitTest.o HashTable.o List.o SymTab.o $(LIBS)

List.o:
	$(CC) $(CCFLAGS) -c ../../List/List.c

HashTable.o:
	$(CC) $(CCFLAGS) -c ../HashTable/HashTable.c

SymTab.o:
	$(CC) $(CCFLAGS) -c SymTab.c

UnitTest.o:
	$(CC) $(CCFLAGS) -c UnitTest.c

clean:
	rm -f *.o $(BIN)
