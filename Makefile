PROG = mangadex_fetcher

CXX = g++

LINKFLAGS = -lcurl -larchive
CXXFLAGS = -g -Wall -MD -std=c++17

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(PROG)

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LINKFLAGS)

clean:
	rm -rf $(PROG) $(OBJ) $(OBJ:.o=.d)

install: $(PROG)
	cp mangadex_fetcher /usr/local/bin/

.PHONY: clean
