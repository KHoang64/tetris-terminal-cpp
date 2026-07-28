CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Build targets
TARGETS = tetris lab_ansi lab_termios lab_arrows

# Default: build everything
all: $(TARGETS)

# Main game
tetris: src/main.cpp
	$(CXX) $(CXXFLAGS) -o tetris src/main.cpp

# Lab 1: ANSI escape codes
lab_ansi: src/lab_ansi.cpp
	$(CXX) $(CXXFLAGS) -o lab_ansi src/lab_ansi.cpp

# Lab 2: termios raw mode
lab_termios: src/lab_termios.cpp
	$(CXX) $(CXXFLAGS) -o lab_termios src/lab_termios.cpp

# Lab 3: Arrow key input
lab_arrows: src/lab_arrows.cpp
	$(CXX) $(CXXFLAGS) -o lab_arrows src/lab_arrows.cpp

clean:
	rm -f $(TARGETS)

.PHONY: all clean
