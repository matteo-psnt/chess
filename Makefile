# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++14 -Iinc -I/opt/homebrew/opt/sfml/include -Wall -Wextra
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

# Source and header files
SRC = $(wildcard src/*.cc)
HEADERS = $(wildcard include/*.h)

# Output directory for object files
BUILD_DIR = build

# Target executable
EXEC = chess

# Object files
OBJ = $(SRC:src/%.cc=$(BUILD_DIR)/%.o)

TEST_EXEC = test

# Rules
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.cc $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_EXEC): test.cc
	$(CXX) test.cc -o $(TEST_EXEC) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all clean
