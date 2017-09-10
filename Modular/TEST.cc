#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

#include "byte_func.h"
#include "benc_funcs.h"

int main() {
	std::ifstream inputfile("testOut.txt", std::ios::binary);
	std::fstream outputfile;

	dencrypt(inputfile, outputfile, "testOut1.txt");


	return 0;
}
