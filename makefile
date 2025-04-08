SOURCE := $(wildcard *.cpp */*.cpp */*/*.cpp)

FLAGS := -Wall -O3 -Wno-unused-variable -fopenmp

INCLUDES := -I /usr/include/eigen3

build:
	g++ -g main.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp -o main.exe $(FLAGS) $(INCLUDES)

debug:
	g++ -g main.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp -o main.exe $(FLAGS) $(INCLUDES) -DDEBUG

tests:
	g++ -g src/graph/graph.test.cpp src/disjoint_sets/disjoint_sets.test.cpp src/spanning_tree/spanning_tree.test.cpp src/util/util.test.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp include/doctest.h -o tests.exe $(FLAGS) $(INCLUDES)

cycle-detection:
	g++ -g cycle_detection.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp -o cycle_detection.exe $(FLAGS) $(INCLUDES)

clean:
	del main.exe
	del graph.txt
	del results.json
	del tests.exe
	del cycle_detection.exe
	del cycle_detection.txt
	del tools/cycle_detection.png

clean-linux:
	-rm main.exe
	-rm graph.txt
	-rm results.json
	-rm tests.exe
	-rm cycle_detection.exe
	-rm cycle_detection.txt
	-rm tools/cycle_detection.png