#
# Tyler Filla
# CS 4280 - P0
#

LDFLAGS=
CXXFLAGS=-std=c++11

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

P0: main.o tree.o
	g++ -o $@ $^ $(LDFLAGS)

all: P0

clean:
	rm -rf *.o

.PHONY: clean
.SECONDARY:
