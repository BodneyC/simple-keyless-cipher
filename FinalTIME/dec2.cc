#include <iostream>
#include <fstream> // ifstream and ofstream
#include <cstdio>
#include <cmath> // pow()
#include <ctime> // Was being used for testing purposes
#include <iomanip> // setbase() in std::cout
#include "stat.h"

#define two255 65025
#define three255 16581375
#define byte 255
/*------------------------------------------------
Converts 64-bit integer to a byte array
------------------------------------------------*/
unsigned char* dectobytes(__int64 value, unsigned char *bytearray) {
  int j = 3;
  for (size_t i=0; i<4; i++) {
    bytearray[j] = value & 0xFF; // Mask with a byte
    value = value >> 8; // Shift by a byte
    j--;
  }
  return bytearray;
}
/*------------------------------------------------
Sums each byte to a sequential power of 255
------------------------------------------------*/
void power255(unsigned char* bytevals, __int64 &value) {
  value = 0;
  for (size_t i = 0; i < 5; i++) {
    value += bytevals[i] * (pow(255.0, i)); // Keep adding array elements * 255^i
  }
}
/*------------------------------------------------
MAIN BODY
------------------------------------------------*/
int main(int argc, char const *argv[]) {
  __int64 value255 = 0;
  unsigned char *bytevals = new unsigned char[5];
  unsigned char *bytevals255 = new unsigned char[4];
  unsigned char *remaining;
    // argc = 3;
    // argv[1] = "encfile.bin";
    // argv[2] = "decfile.png";
  if (argc != 3) { // Check # of arguments
    std::cout << "Usage: program <input_file> <output_file>" << '\n';
    return 1;
  }
  struct stat results;
  if (stat(argv[1], &results) != 0) {  // Use of stat.h for input file information
    std::cout << "Cannot gather input file information" << '\n';
    return 1;
  }
  std::ifstream encfile;
  encfile.open(argv[1], std::ios::binary); // Input file as binary
  std::ofstream decfile;
  decfile.open(argv[2], std::ios::binary | std::ios::trunc); // Output file as binary and truncate
  if(!encfile.is_open() || !decfile.is_open()){ // Checking if both files are open
    std::cout << "Cannot open file" << '\n';
    return 1;
  }
  std::cout << "Starting..." << '\n';

  for (size_t i = 0; i < (results.st_size / 5); i++) { // Loop for # of bytes in file / 5
    encfile.read((char*)bytevals, 5);
    power255(bytevals, value255); // Provide byte-array and sum as though 255 tokens
    dectobytes(value255, bytevals255); // Take value of power255() and convert to byte-array
    decfile.write((char*)bytevals255, 4);
    // delete[] bytevals255;
    // delete[] bytevals;
  }

  int remaint = results.st_size % 5; // Remainder of bytes of file
  remaining = new unsigned char[remaint+1]; // Dynamically allocated array for bytes of remainder
  encfile.read((char*)remaining, remaint);
  for (size_t i = 0; i < remaint; i++) {
    remaining[i] ^= 0xBE; // Xor remainder with 0xBE (not really encryptions but meh)
  }
  decfile.write((char*)remaining, remaint);
  std::cout << "Program Completed" << '\n';
  delete[] bytevals255, bytevals;
  return 0;
}
