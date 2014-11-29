CC=g++
CFLAGS=-W -Wall
SRC=main.cpp
BIN=bin

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
