#!/bin/bash

IN_FILE="$1"
if [[ -z "$IN_FILE" ]]; then
    IN_FILE="./src/main.C"
fi

if [[ $(pwd) =~ test$ ]]; then
	cd ..
fi

make -B

if [[ $? -ne 0 ]]; then
	echo "Compilation failed"
	exit
fi

./bin/genenc "$IN_FILE" ./test/test_a.enc
./bin/genenc -d ./test/test_a.enc ./test/test_a.dec

DIFF=$(diff "$IN_FILE" ./test/test_a.dec)

if ! cmp -s "$IN_FILE" test/test_a.dec; then
	echo "Test failed:"
	echo "$DIFF"
else
	echo "Test passed"
fi
