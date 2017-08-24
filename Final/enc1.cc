#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <climits>
#include <iomanip>
#include "stat.h"

#define two255 65025
#define three255 16581375
#define four255 4228250625
#define byte 255
/*------------------------------------------------
Works down orders of 255
------------------------------------------------*/
unsigned char* workdown(__int64 value255) {
  unsigned char* bytearray = new unsigned char[5];
  __int64 j = 254;
  /* N.B. Repetition of code is to prevent having
  to calculate 255^X god knows how many times */
  if(value255 > four255) {
    bytearray[4] = 1;
    value255 -= four255;
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
Coverts char array to decimal
------------------------------------------------*/
__int64 bytestodec(unsigned char* bytevals) {
  __int64 temp = 0;
  int j = 3;
  for (size_t i = 0; i < 4; i++) {
      temp += bytevals[j] * pow(256.0,i); // Sum: array_element[j] * 256^i
      j--;
  }
  return temp;
}
/*------------------------------------------------
PROGRESS BAR
------------------------------------------------*/
void progress (int size, int i) {
  int barwidth = 50;
  static int pos = 0;

  std::cout << "[";
  if(i % (size/barwidth) == 0) pos++;
  for (int j = 0; j < barwidth; ++j) {
      if (j < pos) std::cout << "=";
      else if (j == pos) std::cout << ">";
      else std::cout << " ";
  }
  std::cout << "]" << pos*2 << "%\r";
  std::cout.flush();
}
/*------------------------------------------------
MAIN BODY
------------------------------------------------*/
int main(int argc, char const *argv[]) {
  __int64 value = 0; // 4244897280 MAX VAL
  __int64 value255 = 0;
  __int64 newvalue = 0;
  unsigned char *bytevals = new unsigned char[4];
  unsigned char *bytevals255, *remaining;

  // argc = 3;
  // argv[1] = "testWRD.docx";
  // argv[2] = "encfile.bin";
  if (argc != 3) {
    std::cout << "Usage: program <input_file> <output_file>" << '\n';
    return 1;
  }

  struct stat results;
  if (stat(argv[1], &results) != 0) { // Gathing input file info into results struct
    std::cout << "Cannot gather input file information" << '\n';
    return 1;
  }
  std::ifstream inputfile;
  inputfile.open(argv[1], std::ios::binary); // Input file as binary
  std::ofstream encfile;
  encfile.open(argv[2], std::ios::binary | std::ios::trunc); // Output stream as binary & truncate
  if(!encfile.is_open() || !inputfile.is_open()){ // Error checking for both files
    std::cout << "Cannot open file" << '\n';
    return 1;
  }

  for (size_t i = 0; i < (results.st_size / 4); i++) { // Loop for # of bytes in file / 4
    if(!inputfile.read((char*)bytevals, 4)) { // Read four bytes and error checking
      std::cout << "Failed to read: iteration " << i << '\n';
      return 1;
    }
    value = bytestodec(bytevals); // Array to decimal: ENCRYPTED
    bytevals255 = workdown(value); // Finds largest value of decreasing powers of 255
    if(!encfile.write((char*)bytevals255, 5)) { // Write four bytes and error checking
      std::cout << "Failed to write: iteration " << i << '\n';
      return 1;
    }
    std::cout << "Completed iteration: " << i << '\n';
    // progress(results.st_size/4, i); // Call progress() for output loading bar
  }
  // std::cout << std::endl; // Newline to follow progress()

  int remaint = results.st_size % 4; // Remainder of bytes of file
  remaining = new unsigned char[remaint+1]; // Dynamically allocated array for bytes of remainder
  if(!inputfile.read((char*)remaining, remaint)) { // Read remaining bytes and error checking
    std::cout << "Failed to read final iteration" << '\n';
  }
  for (size_t i = 0; i < remaint; i++) {
    remaining[i] ^= 0xBE; // Xor remainder with 0xBE
  }
  if(!encfile.write((char*)remaining, remaint)) { // Write remaining bytes and error checking
    std::cout << "Failed to write final iteration " << '\n';
  }
  std::cout << "Program Completed" << '\n';
  return 0;
}
