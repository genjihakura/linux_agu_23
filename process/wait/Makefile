. PHONY: all

CUR_DIR := .
PROJECT := waitpid
INC_DIR :=$(CUR_DIR)/inc
OBJ_DIR :=$(CUR_DIR)/obj
LIB_DIR :=$(CUR_DIR)/lib
SRC_DIR :=$(CUR_DIR)/src
BIN_DIR :=$(CUR_DIR)/bin
LID_STATIC := $(LIB_DIR)/static
LID_SHARE := $(LIB_DIR)/share

IN_FLAG := -I $(INC_DIR)
CC := gcc
SOURCE := main.c
OBJ_FILE := main.o

objs:
	$(CC) -c -o $(OBJ_DIR)/$(OBJ_FILE) -c $(SOURCE) $(IN_FLAG)

all:objs
	$(CC) -o $(BIN_DIR)/$(PROJECT) $(OBJ_DIR)/$(OBJ_FILE)

clean:
	rm -f $(BIN_DIR)/* \
	$(OBJ_DIR)/*
