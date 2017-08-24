#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <iomanip>
#include "stat.h"

#define two255 65025
#define three255 16581375
#define byte 255
//------------------------------------------------
unsigned char* dectobytes(__int64 value) {
  unsigned char* bytearray = new unsigned char[4];
  int j = 3;
  for (size_t i=0; i<4; i++) {
    bytearray[j] = value & 0xFF;
    value = value >> 8;
    j--;
  }
  return bytearray;
}
//------------------------------------------------
void power255(unsigned char* bytevals, __int64 &value) {
  value = 0;
  for (size_t i = 0; i < 5; i++) {      //    !!! 5 !!!
    value += bytevals[i] * (pow(255.0, i));
  }
}
//------------------------------------------------
int main(int argc, char const *argv[]) {
  __int64 value255 = 0;
  unsigned char *bytevals = new unsigned char[5];
  unsigned char *bytevals255, *remaining;

  // argc = 3;
  // argv[1] = "encfile.bin";
  // argv[2] = "decfile.docx";
  if (argc != 3) {
    std::cout << "Usage: program <input_file> <output_file>" << '\n';
    return 1;
  }

  struct stat results;
  if (stat(argv[1], &results) != 0) {
    std::cout << "Cannot gather input file information" << '\n';
    return 1;
  }
  std::ifstream encfile;
  encfile.open(argv[1], std::ios::binary);
  std::ofstream decfile;
  decfile.open(argv[2], std::ios::binary | std::ios::trunc);
  if(!encfile.is_open() || !decfile.is_open()){
    std::cout << "Cannot open file" << '\n';
    return 1;
  }

  for (size_t i = 0; i < (results.st_size / 5); i++) {
    if(!encfile.read((char*)bytevals, 5)) {
      std::cout << "Failed to read: iteration " << i << '\n';
      return 1;
    }
    power255(bytevals, value255);
    bytevals255 = dectobytes(value255);
    if(!decfile.write((char*)bytevals255, 4)) {
      std::cout << "Failed to write: iteration " << i << '\n';
      return 1;
    }
    std::cout << "Completed iteration: " << i << '\n';
  }

  int remaint = results.st_size % 5;
  remaining = new unsigned char[remaint+1];
  if(!encfile.read((char*)remaining, remaint)) {
    std::cout << "Failed to read final iteration" << '\n';
  }
  for (size_t i = 0; i < remaint; i++) {
    remaining[i] ^= 0xBE;
  }
  if(!decfile.write((char*)remaining, remaint)) {
    std::cout << "Failed to write final iteration " << '\n';
  }
  std::cout << "Program Completed: " << '\n';
  return 0;
}
