CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS = -ly -ll -lm

# Directory for Lex and Yacc
GRAMMAR_DIR = LexAndYacc
TREE_DIR = ParseTree

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
GPC_OBJS = ParsePascal.o
TREE_OBJS = List/List.o $(TREE_DIR)/tree.o
SEM_OBJS = SemCheck.o SemCheck_stmt.o SemCheck_expr.o HashTable.o SymTab.o
PARSER_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
ALL_OBJS = $(GPC_OBJS) $(TREE_OBJS) $(SEM_OBJS) $(PARSER_OBJS)

######## THE MAIN BUILD RULES ###########
release: lexAndYacc parser

debug_flexbison: debugFlexBison
debug_bison: bisonDebug parser
debug_flex: flexDebug parser


############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
parser: $(ALL_OBJS) ParsePascal.o

ParsePascal.o: $(PARSER_OBJS) $(TREE_OBJS) ParsePascal.h ParsePascal.c
	$(CC) $(CCFLAGS) -c ParsePascal.c

#ParsePascal.o: $(PARSER_OBJS) $(TREE_OBJS) ParsePascal.h ParsePascal.c
#	$(CC) $(CCFLAGS) -DDEBUG_BISON -c ParsePascal.c

List/List.o:
	$(CC) $(CCFLAGS) -c List/List.c

SemCheck.o: $(PARSER_OBJS) $(TREE_OBJS) SemCheck_stmt.o SemCheck_expr.o SymTab.o HashTable.o
	$(CC) $(CCFLAGS) -c SemanticCheck/SemCheck.c

SemCheck_stmt.o: $(PARSER_OBJS) $(TREE_OBJS) SymTab.o HashTable.o
	$(CC) $(CCFLAGS) -c SemanticCheck/SemChecks/SemCheck_stmt.c

SemCheck_expr.o: $(PARSER_OBJS) $(TREE_OBJS) SymTab.o HashTable.o
	$(CC) $(CCFLAGS) -c SemanticCheck/SemChecks/SemCheck_expr.c

HashTable.o: $(GRAMMAR_DIR)/y.tab.h
	$(CC) $(CCFLAGS) -c SemanticCheck/HashTable/HashTable.c

SymTab.o: $(GRAMMAR_DIR)/y.tab.h HashTable.o
	$(CC) $(CCFLAGS) -c SemanticCheck/SymTab/SymTab.c

$(TREE_DIR)/tree.o:
	$(CC) $(CCFLAGS) -c $(TREE_DIR)/tree.c

lexAndYacc:
	cd $(GRAMMAR_DIR) && $(MAKE)

debugFlexBison:
	cd $(GRAMMAR_DIR) && $(MAKE) flexDebug
	CCFLAGS += -DDEBUG_BISON

bisonDebug:
	cd $(GRAMMAR_DIR) && $(MAKE)
	CCFLAGS += -DDEBUG_BISON

flexDebug:
	cd $(GRAMMAR_DIR) && $(MAKE) flexDebug



############ CLEANING ##########3
clean: clean_parser clean_LexYacc
clean_parser:
	rm -f $(ALL_OBJS) *.o
clean_LexYacc:
	cd $(GRAMMAR_DIR) && $(MAKE) clean
