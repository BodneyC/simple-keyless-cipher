CC=g++
CXXFLAGS=-std=c++11
DEPS=byte_func.h benc_funcs.h
OBJ=byte_func.o benc_funcs.o TEST.o
OUT=TEST

$(OUT): $(OBJ)
	$(CC) $^ -o $@

%.o: %.cc $(DEPS)
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean cleaner

clean:
	\rm -f *.o

cleaner:
	\rm -f *.o *.txt $(OUT)
