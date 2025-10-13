# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -I./interface

# Directories
SRC_DIR := src
INC_DIR := interface
TEST_DIR := tests
OBJ_DIR := build
BIN_DIR := bin

# File patterns
SRC_FILES := $(filter-out src/WorldOfTextCraft.cc, $(wildcard $(SRC_DIR)/*.cc))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_FILES := $(wildcard $(TEST_DIR)/*.cc)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cc, $(TEST_DIR)/%.exe, $(TEST_FILES))

# Default target
all: bin/WorldOfTextCraft.exe $(TEST_BINS)

# Rule to build main executable
bin/WorldOfTextCraft.exe: $(OBJ_FILES)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ src/WorldOfTextCraft.cc $(OBJ_FILES)

# Rule to build test executables
$(TEST_DIR)/%.exe: $(TEST_DIR)/%.cc $(OBJ_FILES) | $(BIN_DIR)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJ_FILES)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Cleanup
clean:
	rm -rf ./$(OBJ_DIR)/*.o ./$(BIN_DIR)/*.exe

# Phony targets
.PHONY: all clean
