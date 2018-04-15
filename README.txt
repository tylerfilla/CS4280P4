Tyler Filla
CS 4280
Project 2

Building
========

Run the following command:
$ make

Using
=====

There are two ways to run the program:
$ ./frontEnd <basename>
$ ./frontEnd

The first way reads file <basename>.sp18 from the CWD and parses it. The second
way reads from standard input. The parse tree is built as characters are read.

The tree will be printed to standard output upon successful parse, which
includes a final EOF token. On delmar, use ^D to send EOF. In the event of a
scanner or parser error, details will be printed to standard error.
