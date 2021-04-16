PROJECT = cpp-web-server

CXX = g++
CXX_FLAGS = -Wall -std=c++11
ifeq ($(DEBUG),1)
    CXX_FLAGS += -O0 -g
else
    CXX_FLAGS += -O3 -g0
endif

LIBS = pthread event

FILES_CPP = $(wildcard src/*.cpp)
FILES_OBJ = $(FILES_CPP:.cpp=.o)

all: clear build

.PHONY: build
build: $(PROJECT)

$(PROJECT): $(FILES_OBJ)
	$(CXX) $(CXX_FLAGS) $(FILES_OBJ) -o $(PROJECT) $(addprefix -l, $(LIBS))

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

.PHONY: clear
clear:
	rm -rf $(FILES_OBJ) $(PROJECT)
