PROJECT = cpp-web-server

CXX = g++
CXX_FLAGS = -Wall -std=c++11 -O3 -g0

LIBS = pthread event

FILES_CPP = $(wildcard src/*.cpp)
FILES_OBJ = $(FILES_CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

all: $(FILES_OBJ)
	$(CXX) $(CXX_FLAGS) $(FILES_OBJ) -o $(PROJECT) $(addprefix -l, $(LIBS))

.PHONY: clean
clear:
	rm -rf $(FILES_OBJ) $(PROJECT)
