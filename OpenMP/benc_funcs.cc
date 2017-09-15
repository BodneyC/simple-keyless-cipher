#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <climits>
#include <iomanip>
#include <inttypes.h>

#include "benc_funcs.h"
#include "byte_func.h"
//------------------------------------------------
#define two255 65025
#define three255 16581375
#define four255 4228250625
/*------------------------------------------------
DEC: Sums each byte to a sequential power of 255
------------------------------------------------*/
void power255(unsigned char* bytevals, int64_t &value) {
	value = 0;
	for (size_t i = 0; i < 5; i++) {
		value += bytevals[i] * (pow(255.0, i)); // Keep adding array elements * 255^i
	}
}
/*------------------------------------------------
ENC: Works down orders of 255
------------------------------------------------*/
unsigned char* workdown(int64_t value255, unsigned char *bytearray) {
	int64_t j = 254;

	/* N.B. Repetition of code is to prevent having
	to calculate 255^X god knows how many times */
	if(value255 > four255) {
		bytearray[4] = 1;
		value255 -= four255;\
	} else { bytearray[4] = 0; }

	// three255 loop
	while (value255 - (j * three255) < 0) j--; // Find x*(255^3) !< 0
	value255 = value255 - (j * three255);
	bytearray[3] = j;

	j = 254;
	// two255 loop
	while (value255 - (j * two255) < 0) j--; // Find x*(255^2) !< 0
	value255 = value255 - (j * two255);
	bytearray[2] = j;

	j = 254;
	// 255 loop
	while (value255 - (j * 255) < 0) j--; // Find x*(255^1) !< 0
	value255 = value255 - (j * 255);
	bytearray[1] = j;

	// final
	bytearray[0] = value255; // Remaining value is itself
	return bytearray;
}
/*------------------------------------------------
ENC: MAIN BODY
------------------------------------------------*/
int bencrypt(std::string inputFile, std::string outputFile) {
	int64_t filesize;
	unsigned char *remaining;
	int i = 0;

	std::ifstream inputfile;
	inputfile.open(inputFile, std::ios::binary); // Input file as binary
	inputfile.seekg(0, inputfile.end);
	filesize = inputfile.tellg();
	inputfile.seekg(0, inputfile.beg);

	std::ofstream encfile;
	encfile.open(outputFile, std::ios::binary | std::ios::trunc); // Output stream as binary & truncate

	if(!encfile.is_open() || !inputfile.is_open()){ // Error checking for both files
		std::cout << "Cannot open file" << '\n';
		return 1;
	}

	std::cout << "Starting..." << '\n';
	#pragma omp parallel for ordered schedule(static,1) lastprivate(i)
		for (i = 0; i < (filesize / 4); i++) { // Loop for # of bytes in file / 4
			unsigned char bytevals[4];
			unsigned char bytevals255[5];
			int64_t value = 0; // 4244897280 MAX VAL

			#pragma omp ordered
			{
				inputfile.seekg(i * 4, inputfile.beg);
				inputfile.read((char*)bytevals, 4);
			}

			value = bytestodec(bytevals, 4); // Array to decimal: ENCRYPTED
			workdown(value, bytevals255); // Finds largest value of decreasing powers of 255

			#pragma omp ordered
			{
				encfile.write((char*)bytevals255, 5);
			}
		}

	inputfile.seekg(i * 4, inputfile.beg);

	int remaint = filesize % 4; // Remainder of bytes of file
	remaining = new unsigned char[remaint+1]; // Dynamically allocated array for bytes of remainder
	inputfile.read((char*)remaining, remaint);

	for (size_t i = 0; i < remaint; i++) {
		remaining[i] ^= 0xBE; // Xor remainder with 0xBE
	}

	encfile.write((char*)remaining, remaint);

	return 0;
}
/*------------------------------------------------
DEC: MAIN BODY
------------------------------------------------*/
int dencrypt(std::string encfileString, std::string decfileString) {
	int64_t filesize;
	unsigned char *remaining;
	int i = 0;

	std::ifstream encfile;
	encfile.open(encfileString, std::ios::binary); // Input file as binary
	if(!encfile.is_open()) {
		std::cout << "Input file is not open";
		return 1;
	}
	encfile.seekg(0, encfile.end);
	filesize = encfile.tellg();
	encfile.seekg(0, encfile.beg);


	int64_t value255 = 0;

	std::ofstream decfile;
	decfile.open(decfileString, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc); // Output file as binary and truncate
	if(!decfile.is_open()){ // Checking if both files are open
		std::cout << "Cannot open output file" << '\n';
		return 1;
	}

	std::cout << "Starting..." << '\n';

	#pragma omp parallel for ordered schedule(static,1) lastprivate(i)
		for (size_t i = 0; i < (filesize / 5); i++) { // Loop for # of bytes in file / 5
			unsigned char bytevals[4];
			unsigned char bytevals255[5];

			#pragma omp ordered
			{
				encfile.seekg(i * 5, encfile.beg);
				encfile.read((char*)bytevals, 4);
			}

			power255(bytevals, value255); // Provide byte-array and sum as though 255 tokens
			dectobytes(value255, bytevals255, 4); // Take value of power255() and convert to byte-array

			#pragma omp ordered
			{
				decfile.write((char*)bytevals255, 4);
			}
		}

	encfile.seekg(i * 4, encfile.beg);

	int remaint = filesize % 5; // Remainder of bytes of file
	remaining = new unsigned char[remaint+1]; // Dynamically allocated array for bytes of remainder

	encfile.read((char*)remaining, remaint);
	for (size_t i = 0; i < remaint; i++) {
		remaining[i] ^= 0xBE; // Xor remainder with 0xBE (not really encryptions but meh)
	}

	decfile.write((char*)remaining, remaint);
	std::cout << "Program Completed" << '\n';

	return 0;
}
