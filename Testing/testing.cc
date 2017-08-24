#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <iomanip> // setbase

#define two255 65025
#define three255 16581375
#define four255 4228250625
#define byte 255
//------------------------------------------------
int* dectobytes(__int64 value) {
  int* bytearray = new int[4];
  for (size_t i=0; i<4; i++) {
    bytearray[i] = value & 0xFF;
    value = value >> 8;
  }
  return bytearray;
}
//------------------------------------------------
void power255(int* bytevals, __int64 &value) {
  value = 0;
  for (size_t i = 0; i < 5; i++) {      //    !!! 5 !!!
    value += bytevals[i] * (pow(255.0, i));
  }
}
//------------------------------------------------
int* workdown(__int64 value255) {
  int* bytearray = new int[5];      //    !!! 5 !!!
  __int64 j = 254;

  //--------------
  if(value255 > four255) {
    bytearray[4] = 1;
    value255 -= four255;
  } else { bytearray[4] = 0; }
  //--------------

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
//------------------------------------------------
__int64 bytestodec(int* bytevals) {
  __int64 temp = 0;
  for (size_t i = 0; i < 5; i++) {
      temp += bytevals[i] * pow(256.0,i);
  }
  return temp;
}
//------------------------------------------------
int main(int argc, char const *argv[]) {
  // __int64 value = 0; // 4244897280 MAX VAL
  // __int64 value255 = 0;
  // __int64 newvalue = 0;
  // int *bytevals, *bytevals255, *newbytevals;
  // int barwidth = 50;
  // int pos = 0;
  // bool tester = 0;
  //
  // std::ofstream outputfile;
  // outputfile.open("output.txt", std::ios::trunc);
  // if(!outputfile.is_open()){
  //   std::cout << "Cannot open file" << '\n';
  //   return 1;
  // }
  //
  // for (size_t i = 0; i < 2000; i++) {
  unsigned char cats = 'C';
  std::cout << "       ------------------------------------------------ : -------------------------------" << '\n' << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(5) << (int)cats;
    // bytevals = dectobytes(value);
    // power255(bytevals, value255);
    // bytevals255 = dectobytes(value255);
    // newbytevals = workdown(value255);
    // newvalue = bytestodec(newbytevals);
    // bytevals = dectobytes(value); // Converts value to byte array
    // bytevals255 = workdown(value); // Finds largest value of decreasing powers of 255
    // value255 = bytestodec(bytevals255); // Array to decimal: ENCRYPTED
    // power255(bytevals255, newvalue); // Forms a decimal with exponents of 255
    // newbytevals = dectobytes(newvalue); // Forms an array: DECRYPTED

    // std::cout << "[";
    // if(i % (2000/barwidth) == 0) pos++;
    // for (int j = 0; j < barwidth; ++j) {
    //     if (j < pos) std::cout << "=";
    //     else if (j == pos) std::cout << ">";
    //     else std::cout << " ";
    // }
    // std::cout << "]" << pos*2 << "%\r";
    // std::cout.flush();


    // for (size_t i = 0; i < 4; i++) {
    //   if(bytevals255[i]>255) tester = 1;
    // }
    // if(tester==1 || value != newvalue) { // If (failure)
      // outputfile << "Original Byte Values: "; // Ouput original
      // for (int j = 3; j>=0; j--)
      //   outputfile << bytevals[j] << ' ';
      // outputfile << std::endl << "Original Value: " << value << std::endl;
      //
      // outputfile << "255 byte values: "; // Output encrypted
      // for (int j = 4; j>=0; j--){
      //   outputfile << bytevals255[j] << ' ';
      // }
      // outputfile << std::endl;
      // for (int j = 4; j>=0; j--){
      //   outputfile << std::setbase(16) << bytevals255[j] << ' ';
      // }
      // outputfile << std::endl << "255 Value: " << value255 << std::endl;
      //
      // outputfile << "Worked Byte Values: "; // Output decrypted
      // for (int j = 3; j>=0; j--)
      //   outputfile << newbytevals[j] << ' ';
      // outputfile << std::endl << "New Value: " << newvalue << std::endl << std::endl;
    // }
    // value++;
    // tester=0;
//     delete[] bytevals, bytevals255;
//   }
// std::cout << std::endl;
//   std::cout << "chicken" << '\n';
  return 0;
}
