#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <climits>
#include <chrono>
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
  unsigned char* bytearray = new unsigned char[5];      //    !!! 5 !!!
  __int64 j = 254;

  if(value255 > four255) {
    bytearray[4] = 1;
    value255 -= four255;
  } else { bytearray[4] = 0; }

  // three255 loop
  while (value255 - (j * three255) < 0) j--; // Find x*(255^3) !<
  value255 = value255 - (j * three255);
  bytearray[3] = j;
  j = 254;
  // two255 loop
  while (value255 - (j * two255) < 0) j--;
  value255 = value255 - (j * two255);
  bytearray[2] = j;
  j = 254;
  // 255 loop
  while (value255 - (j * 255) < 0) j--;
  value255 = value255 - (j * 255);
  bytearray[1] = j;
  // final
  bytearray[0] = value255;
  return bytearray;
}
/*------------------------------------------------
Coverts char array to decimal
------------------------------------------------*/
__int64 bytestodec(unsigned char* bytevals) {
  __int64 temp = 0;
  int j = 3;
  for (size_t i = 0; i < 4; i++) {
      temp += bytevals[j] * pow(256.0,i);
      j--;
  }
  return temp;
}
/*------------------------------------------------
PROGRESS BAR
------------------------------------------------*/
void progress (int size, int i) {
  int barwidth = 40;
  static int pos = 0;

  std::cout << "[";
  if(i % (size/barwidth) == 0) pos++;
  for (int j = 0; j < barwidth; ++j) {
      if (j < pos) std::cout << "=";
      else if (j == pos) std::cout << ">";
      else std::cout << " ";
  }
  std::cout << "]" << "\r";
  std::cout.flush();
}
/*------------------------------------------------
MAIN BODY
------------------------------------------------*/
int main(int argc, char const *argv[]) {
  __int64 value = 0; // 4244897280 MAX VAL
  __int64 value255 = 0;
  __int64 newvalue = 0;
  unsigned char *bytevals = new unsigned char[4096];
  unsigned char *bytevals255, *remaining;
// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  // argc = 3;
  // argv[1] = "input.png";
  // argv[2] = "encfile.bin";
  if (argc != 3) {
    std::cout << "Usage: program <input_file> <output_file>" << '\n';
    // return 1;
  }

  // struct stat results;
  // if (stat(argv[1], &results) != 0) {
  //   std::cout << "Cannot gather input file information" << '\n';
  //   return 1;
  // }
  std::ifstream inputfile;
  inputfile.open("input.png", std::ios::binary);
  std::ofstream encfile;
  encfile.open("output.txt", std::ios::binary | std::ios::trunc);
  if(!encfile.is_open() || !inputfile.is_open()){
    std::cout << "Cannot open file" << '\n';
    return 1;
  }
      std::cout << "Starting..." << '\n';

  // for (size_t i = 0; i < (results.st_size / 4); i++) {
  //   inputfile.read((char*)bytevals, 4);
  //   value = bytestodec(bytevals); // Array to decimal: ENCRYPTED
  //   bytevals255 = workdown(value); // Finds largest value of decreasing powers of 255
  //   encfile.write((char*)bytevals255, 5);
  // }
  //
  // int remaint = results.st_size % 4;
  // remaining = new unsigned char[remaint+1];
  // inputfile.read((char*)remaining, remaint);
  // for (size_t i = 0; i < remaint; i++) {
  //   remaining[i] ^= 0xBE;
  // }
  // encfile.write((char*)remaining, remaint);
  // std::cout << "Program Completed" << '\n';
  //           std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
  //           std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() <<std::endl;
            /* Before:
                  10290644, 9918455, 9670520
              After:
                  10290644
            */
  delete[] bytevals;
  return 0;
}
