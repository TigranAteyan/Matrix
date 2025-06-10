CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src

GTEST_FLAGS = -lgtest -lgtest_main -pthread

TEST_SRC = tests/test.cpp
TARGET = build/test_exec

.PHONY: all test clean build_dir

all: $(TARGET)

$(TARGET): $(TEST_SRC) | build_dir
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TARGET) $(GTEST_FLAGS)

build_dir:
	mkdir -p build

test: all
	./$(TARGET)

clean:
	rm -rf build
