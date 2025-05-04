CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude -Itest
LDFLAGS = 

# Windows-specific commands
RM = cmd /C del /Q /F
RMDIR = cmd /C rmdir /Q /S
MKDIR = cmd /C mkdir

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_FILES))

TARGET = $(BIN_DIR)/program.exe
TEST_TARGET = $(BIN_DIR)/tests.exe

.PHONY: all build test clean run run-tests

all: build

build: $(TARGET)

test: $(TEST_TARGET)

$(TARGET): $(SRC_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(filter-out $(OBJ_DIR)/main.o,$(SRC_OBJS)) $(TEST_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	$(MKDIR) "$@"

clean:
	$(RM) "$(BIN_DIR)\*.exe" 2> nul || exit 0
	$(RM) "$(OBJ_DIR)\*.o" 2> nul || exit 0

run: build
	$(TARGET)

run-tests: test
	$(TEST_TARGET)