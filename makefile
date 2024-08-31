SOURCE := $(wildcard *.cpp */*.cpp */*/*.cpp)
OUTPUT := main.exe

FLAGS := -Wall -O3 -Wno-unused-variable -fopenmp

build:
	g++ -g main.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp -o $(OUTPUT) $(FLAGS)

tests:
	g++ -g src/graph/graph.test.cpp src/disjoint_sets/disjoint_sets.test.cpp src/spanning_tree/spanning_tree.test.cpp src/util/util.test.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp include/doctest.h -o tests.exe $(FLAGS)

clean:
	del graph.txt
	del main.exe
	del tests.exe
	del log*.txt
	del result*.json