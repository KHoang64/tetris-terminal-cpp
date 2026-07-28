CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = tetris
SRCS     = src/main.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: clean
