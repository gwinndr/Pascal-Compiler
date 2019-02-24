LEX = flex -I
YACC = byacc
CC = gcc
FLAGS = -g

# Debug defines
FLEX_DEBUG = -DDEBUG_FLEX

all: lex.yy.o y.tab.o
FlexDebug: debug_lex.yy.o y.tab.o

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CCFLAGS) -c lex.yy.c
y.tab.o: y.tab.c y.tab.h
	$(CC) $(CCFLAGS) -c lex.yy.c

debug_lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CCFLAGS) $(FLEX_DEBUG) -c lex.yy.c

y.tab.c y.tab.h: Grammar.y
	$(YACC) -d Grammar.y

lex.yy.c: Tokenizer.l
	$(LEX) Tokenizer.l

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h
