CC=g++
CFLAGS=-std=c++11 -lm
INCLUDE=-I./includes
SRC=$(wildcard src/*.C)
OBJ=$(patsubst src/%.C, obj/%.o, $(SRC))
PROG=./bin/genenc

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

obj/%.o: src/%.C
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

.PHONY: run clean cleaner

run:
	$(PROG) ./scripts/a.out ./scripts/a.enc
clean:
	rm $(OBJ)

cleaner:
	rm $(OBJ) $(PROG)
