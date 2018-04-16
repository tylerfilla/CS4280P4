#
# Tyler Filla
# CS 4280
# Project 3
#

LDFLAGS=
CXXFLAGS=-std=c++11

.PHONY: all
all: tablegen statSem

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

statSem: main.o
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^
	./tablegen table.txt scanner_table.gen.h

.PHONY: clean
clean:
	rm -rf *.o
	rm -f statSem
	rm -f tablegen

.SECONDARY:
