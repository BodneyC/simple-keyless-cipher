#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <climits>
#include <iomanip>
#include <inttypes.h>

#define two255 65025
#define three255 16581375
#define four255 4228250625
#define byte 255

/*------------------------------------------------
 * Works down orders of 255
 *------------------------------------------------*/
unsigned char* workdown(int64_t value255, unsigned char *bytearray)
{
    int64_t j = 254;

    /* N.B. Repetition of code is to prevent having
    to calculate 255^X god knows how many times */
    if(value255 > four255) {
        bytearray[4] = 1;
        value255 -= four255;
    } else {
        bytearray[4] = 0;
    }

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
 * Coverts char array to decimal
 *------------------------------------------------*/
int64_t bytestodec(unsigned char* bytevals)
{
    int64_t temp = 0;
    int j = 3;

    for (size_t i = 0; i < 4; i++) {
        temp += bytevals[j] * pow(256.0, i); // Sum: array_element[j] * 256^i
        j--;
    }

    return temp;
}

/*------------------------------------------------
 * MAIN BODY
 *------------------------------------------------*/
int main(int argc, char const *argv[])
{
    int64_t value = 0; // 4244897280 MAX VAL
    int64_t filesize;
    unsigned char *bytevals = new unsigned char[4];
    unsigned char *bytevals255 = new unsigned char[5];
    unsigned char *remaining;

    if (argc != 3) {
        std::cout << "Usage: program <input_file> <output_file>" << '\n';
        return 1;
    }

    std::ifstream inputfile;
    inputfile.open(argv[1], std::ios::binary); // Input file as binary
    inputfile.seekg(0, inputfile.end);
    filesize = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    std::ofstream encfile;
    encfile.open(argv[2], std::ios::binary | std::ios::trunc); // Output stream as binary & truncate

    if(!encfile.is_open() || !inputfile.is_open()) { // Error checking for both files
        std::cout << "Cannot open file" << '\n';
        return 1;
    }

    std::cout << "Starting..." << '\n';

    for (size_t i = 0; i < (filesize / 4); i++) { // Loop for # of bytes in file / 4
        inputfile.read((char*)bytevals, 4);
        value = bytestodec(bytevals); // Array to decimal: ENCRYPTED
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
