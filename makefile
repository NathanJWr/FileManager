EXE = file_manager

SRC_DIR = src
OBJ_DIR = obj
CC = g++

SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude
CFLAGS += -lstdc++fs -Wall -Werror -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wold-style-cast -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wuseless-cast -std=c++17 -g
LDFLAGS += -lstdc++fs -Llib -lSDL2 -lSDL2_ttf
LDLIBS +=

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

clean:
	$(RM) $(OBJ) $(EXE)
