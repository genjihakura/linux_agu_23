.PHONY: all

CUR_DIR := .

PROJECT := hello

INC_DIR := $(CUR_DIR)/inc
BIN_DIR := $(CUR_DIR)/bin
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC := $(LIB_DIR)/static
LIB_SHARE := $(LIB_DIR)/share

INC_FLAG := -I $(INC_DIR)

CC := gcc

objs:
	${CC} -c -o $(OBJ_DIR)/hello_world.o $(SRC_DIR)/hello_world.c $(INC_FLAG)
	${CC} -c -o $(OBJ_DIR)/hello_phat.o $(SRC_DIR)/hello_phat.c $(INC_FLAG)
	${CC} -c -o $(OBJ_DIR)/main.o main.c $(INC_FLAG)

mkstatic: objs
	ar rcs $(LIB_STATIC)/libhello.a $(OBJ_DIR)/hello_phat.o $(OBJ_DIR)/hello_world.o

all: mkstatic
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_STATIC) -l$(PROJECT) -o $(BIN_DIR)/$(PROJECT)

clean:
	rm -f $(OBJ_DIR)/* \
	$(BIN_DIR)/* \
	$(LIB_STATIC)/* 
