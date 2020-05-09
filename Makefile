PROG = mangadex_fetcher

CXX = g++

LINKFLAGS = -lcurl
CXXFLAGS = -g -Wall -std=c++11

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(PROG)

$(PROG): $(OBJ)
	$(CXX) $(LINKFLAGS) $(CXXFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf $(PROG) $(OBJ)

