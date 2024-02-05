CC = g++
CFLAGS = -Wall

ALL: parser.o compile.o
	$(CC) $(CFLAGS) -o l2cpp parser.o compile.o

Parser: parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp -o parser

Compile: compile.cpp
	$(CC) $(CFLAGS) -c compile.cpp -o compile