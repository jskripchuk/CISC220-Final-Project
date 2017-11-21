#CXX = g++
CXXFLAGS = -O2 -std=c++11

LIBS =
LDLIBS =
INCLUDES =

# force clean build every time
all: clean parse

parse: ChordGraph.o Parse.o
	$(CXX) $^ -o $@

# name a variable sources for all source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f *.exe
