CC=g++
CXXFLAGS=-g -std=c++11
OUT_FILES=genenc gendec

all: $(OUT_FILES)

$(OUT_FILES): %: %.o
	$(CC) $(CXXFLAGS) $< -o $@
	
.PHONY: clean cleaner

clean:
	rm *.o

cleaner:
	rm *.o $(OUT_FILES)
