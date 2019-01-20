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

.PHONY: run_enc run_dec clean cleaner

run_enc:
	$(PROG) ./scripts/a.out ./scripts/a.enc

run_dec:
	$(PROG) -d ./scripts/a.enc ./scripts/a.dec

clean:
	rm $(OBJ)

cleaner:
	rm $(OBJ) $(PROG)
