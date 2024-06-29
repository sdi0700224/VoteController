# Compiler
CC := g++
# Compiler flags: Wall (enable warnings), Werror (treat warnings as errors), g (include debugging symbols)
CPPFLAGS := -Wall -Werror -g
# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include
# Executable name
EXEC := $(BIN_DIR)/MVote
# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
# Arguments for running the program
ARGS := -f voters5000.csv -b 2

# Targets
all: $(EXEC)

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(EXEC) $(CPPFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CPPFLAGS) -I$(INCLUDE_DIR)

run: $(EXEC)
	$(EXEC) $(ARGS)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

debug: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)

# Create necessary directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all run clean debug
