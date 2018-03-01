#
# Tyler Filla
# CS 4280 - P1
#

LDFLAGS=
CXXFLAGS=-std=c++11

.PHONY: all
all: tablegen scanner

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

scanner: main.o
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^
	./tablegen table.txt scanner_table.gen.h

.PHONY: clean
clean:
	rm -rf *.o
	rm -f scanner
	rm -f tablegen

.SECONDARY:
