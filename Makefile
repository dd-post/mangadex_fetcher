PROG = mangadex_fetcher

CXX = g++

LINKFLAGS = -lcurl
CXXFLAGS = -g -Wall -MD -std=c++11

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(PROG)

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG): $(OBJ)
	$(CXX) $(LINKFLAGS) $(CXXFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf $(PROG) $(OBJ) $(OBJ:.o=.d)


-include $(OBJ:.o=.d)
