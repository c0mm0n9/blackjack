CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS =

# Source files for the main executable (excluding tests)
SRC = src/main.cpp \
      src/Card.cpp \
      src/Hand.cpp \
      src/Deck.cpp \
      src/Player.cpp \
      src/HumanPlayer.cpp \
      src/Dealer.cpp \
      src/GameManager.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = blackjack

# Default target: build release version
all: release

# Release build with optimizations
release: CXXFLAGS += -O2
release: $(TARGET)

# Debug build with symbols
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (not actual files)
.PHONY: all release debug clean
