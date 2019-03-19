CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS = -ly -ll -lm

# Directory for Lex and Yacc
GRAMMAR_DIR = LexAndYacc

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
GPC_OBJS = ParsePascal.o
TREE_OBJS = TreeFifoList.o tree.o
PARSER_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
ALL_OBJS = $(GPC_OBJS) $(TREE_OBJS) $(PARSER_OBJS)

######## THE MAIN BUILD RULES ###########
release: lexAndYacc parser

debug_flex: flexDebug parser


############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
parser: $(ALL_OBJS)

ParsePascal.o: $(PARSER_OBJS) $(TREE_OBJS) ParsePascal.h ParsePascal.c
	$(CC) $(CCFLAGS) -c ParsePascal.c

tree.o:
	$(CC) $(CCFLAGS) -c ParseTree/tree.c

TreeFifoList.o:
	$(CC) $(CCFLAGS) -c ParseTree/TreeFifoList/TreeFifoList.c

lexAndYacc:
	cd $(GRAMMAR_DIR) && $(MAKE)

flexDebug:
	cd $(GRAMMAR_DIR) && $(MAKE) flexDebug



############ CLEANING ##########3
clean: clean_parser clean_LexYacc
clean_parser:
	rm -f *.o
clean_LexYacc:
	cd $(GRAMMAR_DIR) && $(MAKE) clean
