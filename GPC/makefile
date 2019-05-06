CC = gcc
FLAGS = -g
OPTIMIZE = -O3
LIBS = -ly -ll -lm

PARSER_DIR = Parser
GRAMMAR_DIR = Parser/LexAndYacc
TREE_DIR = Parser/ParseTree
CODEGEN_DIR = CodeGenerator

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
GPC_OBJS = main.o flags.o
PARSER_OBJS = $(PARSER_DIR)/ParsePascal.o
TREE_OBJS = $(PARSER_DIR)/List.o $(PARSER_DIR)/tree.o
SEM_OBJS = $(PARSER_DIR)/SemCheck.o $(PARSER_DIR)/HashTable.o $(PARSER_DIR)/SymTab.o
SEM_OBJS_MORE = $(PARSER_DIR)/SemCheck_stmt.o $(PARSER_DIR)/SemCheck_expr.o
GRAMMAR_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
CODEGEN_OBJS = $(CODEGEN_DIR)/codegen.o $(CODEGEN_DIR)/stackmng.o $(CODEGEN_DIR)/expr_tree.o
ALL_OBJS = $(GPC_OBJS) $(GRAMMAR_OBJS) $(PARSER_OBJS) $(TREE_OBJS) $(SEM_OBJS) $(SEM_OBJS_MORE) $(CODEGEN_OBJS)

BIN = gpc

######## THE MAIN BUILD RULES ###########
release: parser codegen bin

debug_flexbison: debugFlexBison bin
debug_flex: flexDebug bin
debug_bison: bisonDebug bin


############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
bin: $(ALL_OBJS)
	$(CC) $(CCFLAGS) -o $(BIN) $(ALL_OBJS) $(LIBS)

main.o: $(PARSER_OBJS) main.c
	$(CC) $(CCFLAGS) -c main.c

flags.o:
	$(CC) $(CCFLAGS) -c flags.c

parser:
	cd $(PARSER_DIR) && $(MAKE)

codegen:
	cd $(CODEGEN_DIR) && $(MAKE)

debugFlexBison:
	cd $(PARSER_DIR) && $(MAKE) debug_flexbison

flexDebug:
	cd $(PARSER_DIR) && $(MAKE) debug_flex

bisonDebug:
	cd $(PARSER_DIR) && $(MAKE) debug_bison



############ CLEANING ##########3
clean: clean_gpc clean_LexYacc clean_CodeGen
clean_gpc:
	rm -f *.o $(BIN)
clean_LexYacc:
	cd $(PARSER_DIR) && $(MAKE) clean
clean_CodeGen:
	cd $(CODEGEN_DIR) && $(MAKE) clean
