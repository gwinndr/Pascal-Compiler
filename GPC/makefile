CC = gcc
FLAGS = -g
OPTIMIZE =
LIBS = -ly -ll -lm

PARSER_DIR = Parser
GRAMMAR_DIR = Parser/LexAndYacc

CCFLAGS = $(FLAGS) $(OPTIMIZE)

# Object files to build
GPC_OBJS = main.o
PARSER_OBJS = $(PARSER_DIR)/ParsePascal.o
GRAMMAR_OBJS = $(GRAMMAR_DIR)/lex.yy.o $(GRAMMAR_DIR)/y.tab.o
ALL_OBJS = $(GPC_OBJS) $(GRAMMAR_OBJS) $(PARSER_OBJS)

BIN = gpc

######## THE MAIN BUILD RULES ###########
release: parser bin

debug_all: debug_flex
debug_flex: flexDebug bin


############ MAKING OBJS #############
# Making all the objects and bins across directories
# Please no touch :)
bin: $(ALL_OBJS)
	$(CC) $(CCFLAGS) -o $(BIN) $(ALL_OBJS) $(LIBS)

main.o: $(PARSER_OBJS) main.c
	$(CC) $(CCFLAGS) -c main.c

parser:
	cd $(PARSER_DIR) && $(MAKE)

flexDebug:
	cd $(PARSER_DIR) && $(MAKE) flexDebug



############ CLEANING ##########3
clean: clean_gpc clean_LexYacc
clean_gpc:
	rm -f *.o $(BIN)
clean_LexYacc:
	cd $(PARSER_DIR) && $(MAKE) clean
