CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRCS = main.cpp career.cpp comparison.cpp roadmap.cpp graph.cpp file_handler.cpp utils.cpp ui.cpp

TARGET = drona

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean