#
# Tyler Filla
# CS 4280 - P1
#

LDFLAGS=
CXXFLAGS=-std=c++11

.PHONY: all
all: scanner

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

scanner: main.o
	g++ -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o

.SECONDARY:
