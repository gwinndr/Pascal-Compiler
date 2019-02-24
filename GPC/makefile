CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS = -ly -ll -lm

# Directory for Lex and Yacc
GRAMMAR_DIR = Parser/LexAndYacc

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
GPC_OBJS = main.o
PARSER_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
ALL_OBJS = $(GPC_OBJS) $(PARSER_OBJS)

BIN = gpc

######## THE MAIN BUILD RULES ###########
release: LexAndYacc bin


debug_all: debug_flex
debug_flex: FlexDebug bin


############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
bin: $(ALL_OBJS)
	$(CC) $(CCFLAGS) -o $(BIN) $(ALL_OBJS) $(LIBS)

main.o: $(PARSER_OBJS) main.c
	$(CC) $(CCFLAGS) -c main.c

LexAndYacc:
	cd $(GRAMMAR_DIR) && $(MAKE)
FlexDebug:
	cd $(GRAMMAR_DIR) && $(MAKE) FlexDebug



############ CLEANING ##########3
clean: clean_gpc clean_LexYacc
clean_gpc:
	rm -f *.o $(BIN)
clean_LexYacc:
	cd Parser/LexAndYacc && $(MAKE) clean
