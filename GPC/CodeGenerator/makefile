CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS =

PARSER_DIR = Parser
GRAMMAR_DIR = Parser/LexAndYacc
TREE_DIR = Parser/ParseTree

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
TREE_OBJS = $(PARSER_DIR)/List.o $(PARSER_DIR)/tree.o
GRAMMAR_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
ALL_OBJS = $(GPC_OBJS) $(GRAMMAR_OBJS) $(PARSER_OBJS) $(TREE_OBJS) $(SEM_OBJS) $(SEM_OBJS_MORE)

######## THE MAIN BUILD RULES ###########
Intel_x86-64: Intel_x86-64/codegen.o Intel_x86-64/stackmng/stackmng.o Intel_x86-64/expr_tree/expr_tree.o

############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
Intel_x86-64/codegen.o:
	$(CC) $(CCFLAGS) -c Intel_x86-64/codegen.c

Intel_x86-64/stackmng/stackmng.o:
	$(CC) $(CCFLAGS) -c Intel_x86-64/stackmng/stackmng.c

Intel_x86-64/expr_tree/expr_tree.o:
	$(CC) $(CCFLAGS) -c Intel_x86-64/expr_tree/expr_tree.c



############ CLEANING ##########3
clean:
	rm *.o
