EXE = file_manager

SRC_DIR = src
OBJ_DIR = obj
CC = g++

SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude 
CFLAGS += -lstdc++fs -Wall -Werror -std=c++17 -pedantic-errors -g 
LDFLAGS += -lstdc++fs -Llib 
LDLIBS += 

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

clean:
	$(RM) $(OBJ) $(EXE)
