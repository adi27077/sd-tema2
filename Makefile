CC = gcc
CFLAGS = -Wall -g

SRC = netflix.c functiiLG.c functiiCoadaL.c functiiStivaL.c
EXE = tema2 

build:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -rf $(EXE)
