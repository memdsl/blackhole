NAME     = conf
INC_DIR += .
OBJ_DIR := build

CC = gcc

SRCS   += confdata.c expr.c preprocess.c symbol.c util.c
SRCS   += $(OBJ_DIR)/lexer.lex.c $(OBJ_DIR)/parser.tab.c
CFLAGS += -DYYDEBUG

ifeq ($(NAME), conf)
    SRCS += conf.c
else ifeq ($(NAME), mconf)
    SRCS += mconf.c $(shell find lxdialog/ -name "*.c")
    LIBS += -lncurses
else
    $(error bad target=$(NAME))
endif

include $(METEOR_ENV_HOME)/core/script/build.mk

$(OBJ_DIR)/lexer.lex.o: $(OBJ_DIR)/parser.tab.h

$(OBJ_DIR)/lexer.lex.c: lexer.l $(OBJ_DIR)/parser.tab.h
	@echo + LEX $@
	@flex -o $@ $<

$(OBJ_DIR)/parser.tab.c $(OBJ_DIR)/parser.tab.h: parser.y
	@echo + YACC $@
	@bison -v $< --defines=$(OBJ_DIR)/parser.tab.h -o $(OBJ_DIR)/parser.tab.c

.PHONY: conf mconf

conf:
	@$(MAKE) -s

mconf:
	@$(MAKE) -s NAME=mconf
