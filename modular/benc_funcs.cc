#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <climits>
#include <iomanip>
#include <inttypes.h>
#include "benc_funcs.h"
#include "byte_func.h"

#define two255 65025
#define three255 16581375
#define four255 4228250625

/*------------------------------------------------
 * DEC: Sums each byte to a sequential power of 255
 *------------------------------------------------*/
void power255(unsigned char* bytevals, int64_t &value)
{
    value = 0;

    for (size_t i = 0; i < 5; i++)
        value += bytevals[i] * (pow(255.0, i)); // Keep adding array elements * 255^i
}

/*------------------------------------------------
 * ENC: Works down orders of 255
 *------------------------------------------------*/
unsigned char* workdown(int64_t value255, unsigned char *bytearray)
{
    int64_t j = 254;

    /* N.B. Repetition of code is to prevent having
    to calculate 255^X god knows how many times */
    if(value255 > four255) {
        bytearray[4] = 1;
        value255 -= four255;
    } else
        bytearray[4] = 0;

    // three255 loop
    while (value255 - (j * three255) < 0)
        j--; // Find x*(255^3) !< 0

    value255 = value255 - (j * three255);
    bytearray[3] = j;

    j = 254;

    // two255 loop
    while (value255 - (j * two255) < 0)
        j--; // Find x*(255^2) !< 0

    value255 = value255 - (j * two255);
    bytearray[2] = j;

    j = 254;

    // 255 loop
    while (value255 - (j * 255) < 0)
        j--; // Find x*(255^1) !< 0

    value255 = value255 - (j * 255);
    bytearray[1] = j;

    // final
    bytearray[0] = value255; // Remaining value is itself
    return bytearray;
}

/*------------------------------------------------
 * ENC: MAIN BODY
 *------------------------------------------------*/
int bencrypt(std::ifstream& inputfile, std::fstream& encfile, std::string filename)
{
    int64_t value = 0; // 4244897280 MAX VAL
    int64_t filesize;
    unsigned char *bytevals = new unsigned char[4];
    unsigned char *bytevals255 = new unsigned char[5];
    unsigned char *remaining;

    if(!inputfile.is_open()) {
        std::cout << "Input file is not open";
        return 1;
    }

    inputfile.seekg(0, inputfile.end);
    filesize = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    encfile.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc); // Output stream as binary & truncate

    if(!encfile.is_open()) { // Error checking for both files
        std::cout << "Cannot open output file" << '\n';
        return 1;
    }

    std::cout << "Starting..." << '\n';

    for (size_t i = 0; i < (filesize / 4); i++) { // Loop for # of bytes in file / 4
        inputfile.read((char*)bytevals, 4);
        value = bytestodec(bytevals, 4); // Array to decimal: ENCRYPTED
        workdown(value, bytevals255); // Finds largest value of decreasing powers of 255
        encfile.write((char*)bytevals255, 5);
    }

    int remaint = filesize % 4; // Remainder of bytes of file
    remaining = new unsigned char[remaint + 1]; // Dynamically allocated array for bytes of remainder

    inputfile.read((char*)remaining, remaint);

    for (size_t i = 0; i < remaint; i++)
        remaining[i] ^= 0xBE; // Xor remainder with 0xBE

    encfile.write((char*)remaining, remaint);
    std::cout << "Program Completed" << '\n';
    delete[] bytevals255, bytevals;
    return 0;
}

/*------------------------------------------------
 * DEC: MAIN BODY
 *------------------------------------------------*/
int dencrypt(std::ifstream& encfile, std::fstream& decfile, std::string filename)
{
    int64_t value255 = 0;
    int64_t filesize;
    unsigned char *bytevals = new unsigned char[5];
    unsigned char *bytevals255 = new unsigned char[4];
    unsigned char *remaining;

    if(!encfile.is_open()) {
        std::cout << "Input file is not open";
        return 1;
    }

    encfile.seekg(0, encfile.end);
    filesize = encfile.tellg();
    encfile.seekg(0, encfile.beg);

    decfile.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc); // Output file as binary and truncate

    if(!decfile.is_open()) { // Checking if both files are open
        std::cout << "Cannot open output file" << '\n';
        return 1;
    }

    std::cout << "Starting..." << '\n';

    for (size_t i = 0; i < (filesize / 5); i++) { // Loop for # of bytes in file / 5
        encfile.read((char*)bytevals, 5);
        power255(bytevals, value255); // Provide byte-array and sum as though 255 tokens
        dectobytes(value255, bytevals255, 4); // Take value of power255() and convert to byte-array
        decfile.write((char*)bytevals255, 4);
    }

    int remaint = filesize % 5; // Remainder of bytes of file
    remaining = new unsigned char[remaint + 1]; // Dynamically allocated array for bytes of remainder

    encfile.read((char*)remaining, remaint);

    for (size_t i = 0; i < remaint; i++)
        remaining[i] ^= 0xBE; // Xor remainder with 0xBE (not really encryptions but meh)

    decfile.write((char*)remaining, remaint);
    std::cout << "Program Completed" << '\n';

    delete[] bytevals255, bytevals;
    return 0;
}
