CC = g++

CFLAGS = -Wall
SRC = main.cpp

build:
	$(CC) $(CFLAGS) -o KillerSudoku $(SRC) && ./KillerSudoku

default:
	$(CC) $(CFLAGS) -o KillerSudoku $(SRC) && ./KillerSudoku

run:
	./KillerSudoku

clean:
	rm main.o KillerSudoku