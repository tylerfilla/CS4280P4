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
	make tablegen
	./tablegen
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o

.SECONDARY:
