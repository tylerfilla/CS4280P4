#
# Tyler Filla
# CS 4280 - P1
#

LDFLAGS=
CXXFLAGS=-std=c++11

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

P1: main.o tree.o
	g++ -o $@ $^ $(LDFLAGS)

all: P0

clean:
	rm -rf *.o

.PHONY: clean
.SECONDARY:
