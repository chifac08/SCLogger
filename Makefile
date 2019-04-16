#Vars
INCL_DIR=incl
SRC_DIR=src
OBJ_DIR=obj
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INCL=$(wildcard $(INCL_DIR)/*.h)
CC=gcc
EXE=sclogger.a
CPPFLAGS=-Iincl
CFLAGS=-Wall
AR=ar
RANLIB=ranlib

.PHONY: all clean

all: $(EXE)
$(EXE): $(OBJ)
	$(AR) ru $@ $^
	$(RANLIB) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -O0 -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
