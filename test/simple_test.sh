#!/bin/bash

if [[ $(pwd) =~ test$ ]]; then
	cd ..
fi

make -B

if [[ $? -ne 0 ]]; then
	echo "Compilation failed"
	exit
fi

./bin/genenc ./src/main.C ./test/test_a.enc
./bin/genenc -d ./test/test_a.enc ./test/test_a.dec

DIFF=$(diff ./src/main.C ./test/test_a.dec)

if [[ "$DIFF" -ne "" ]]; then
	echo "Test failed:"
	echo "$DIFF"
else
	echo "Test passed"
fi
