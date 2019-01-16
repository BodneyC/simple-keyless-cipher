CFLAGS=-std=c++11
SRC=$(wildcard *.C)
PROGS=$(patsubst %.C, %, $(SRC))

$(PROGS): $(SRC)
	g++ $(CFLAGS) -o $@ $<

