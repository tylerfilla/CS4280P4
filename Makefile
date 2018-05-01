#
# Tyler Filla
# CS 4280
# Project 4
#

LDFLAGS=
CXXFLAGS=-std=c++11 -g

.PHONY: all
all: tablegen comp

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

comp: main.o tree.o codegen.o
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^
	./tablegen table.txt scanner_table.gen.h

.PHONY: clean
clean:
	rm -rf *.o
	rm -f comp
	rm -f tablegen

.SECONDARY:
