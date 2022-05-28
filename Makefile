#
# Tyler Filla
# CS 4280
# Project 4
#

LDFLAGS=
CXXFLAGS=-std=c++17

.PHONY: all
all: tablegen comp interpret

%.o: %.cpp
	g++ -g $(CXXFLAGS) -o $@ -c $^

comp: main.o tree.o codegen.o
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^
	./tablegen table.txt scanner_table.gen.h

interpret: interpret.o
	g++ -g -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o
	rm -f comp
	rm -f interpret
	rm -f tablegen

.SECONDARY:
