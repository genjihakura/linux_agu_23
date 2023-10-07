. PHONY: all

CUR_DIR := .
BIN_SERVER := server
BIN_CLIENT := client
INC_DIR :=$(CUR_DIR)/inc
OBJ_DIR :=$(CUR_DIR)/obj
LIB_DIR :=$(CUR_DIR)/lib
SRC_DIR :=$(CUR_DIR)/src
BIN_DIR :=$(CUR_DIR)/bin
LID_STATIC := $(LIB_DIR)/static
LID_SHARE := $(LIB_DIR)/share

IN_FLAG := -I $(INC_DIR)
PTHREAD := -pthread
CC := gcc
SERVER := server.c
CLIENT := client.c
OBJ_SERVER := server.o
OBJ_CLIENT := client.o

objs:
	$(CC) -c -o $(OBJ_DIR)/$(OBJ_SERVER) -c $(SERVER) $(IN_FLAG) $(PTHREAD)
	$(CC) -c -o $(OBJ_DIR)/$(OBJ_CLIENT) -c $(CLIENT) $(IN_FLAG) $(PTHREAD)

all:objs
	$(CC) -o $(BIN_DIR)/$(BIN_SERVER) $(OBJ_DIR)/$(OBJ_SERVER) $(PTHREAD)
	$(CC) -o $(BIN_DIR)/$(BIN_CLIENT) $(OBJ_DIR)/$(OBJ_CLIENT) $(PTHREAD)

clean:
	rm -f $(BIN_DIR)/* \
	$(OBJ_DIR)/*
