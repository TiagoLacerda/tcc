SOURCE := $(wildcard *.cpp */*.cpp */*/*.cpp)
OUTPUT := main.exe

FLAGS := -Wall -O3

build:
	# g++ -g $(SOURCE) -o $(OUTPUT) $(FLAGS)
	g++ -g main.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp -o $(OUTPUT) $(FLAGS)

tests: 
	# g++ -g src/graph/graph.test.cpp src/graph/graph.cpp include/doctest.h -o src/graph/graph.test.exe $(FLAGS)

	# g++ -g src/disjoint_sets/disjoint_sets.test.cpp src/disjoint_sets/disjoint_sets.cpp include/doctest.h -o src/disjoint_sets/disjoint_sets.test.exe $(FLAGS)

	g++ -g src/graph/graph.test.cpp src/disjoint_sets/disjoint_sets.test.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp include/doctest.h -o test.exe $(FLAGS)