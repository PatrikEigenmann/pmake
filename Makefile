# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c99 -Iinclude
LDFLAGS :=

# Directories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := build

# Files
TARGET  := $(BIN_DIR)/pmake
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Default build
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)