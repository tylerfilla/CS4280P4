#
# Tyler Filla
# CS 4280
# Project 2
#

LDFLAGS=
CXXFLAGS=-std=c++11 -g

.PHONY: all
all: tablegen frontEnd

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $^

frontEnd: main.o
	g++ -o $@ $^ $(LDFLAGS)

tablegen: tablegen.o
	g++ -o $@ $^
	./tablegen table.txt scanner_table.gen.h

.PHONY: clean
clean:
	rm -rf *.o
#	rm -f frontEnd
#	rm -f tablegen

.SECONDARY:
