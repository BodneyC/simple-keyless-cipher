#include <iostream>
#include <cstdint>
#include <cmath>
#include "byte_func.h"

/*------------------------------------------------
Coverts char array to decimal
------------------------------------------------*/
int64_t bytestodec(unsigned char* bytevals, int n) {
  int64_t temp = 0;
  int j = n-1;
  for (size_t i = 0; i < n; i++) {
    temp += bytevals[j] * pow(256.0,i); // Sum: array_element[j] * 256^i
    j--;
  }
  return temp;
}


/*------------------------------------------------
Converts 64-bit integer to a byte array
------------------------------------------------*/
unsigned char* dectobytes(int64_t value, unsigned char *bytearray, int n) {
  int j = n-1;
  for (size_t i=0; i<n; i++) {
    bytearray[j] = value & 0xFF; // Mask with a byte
    value = value >> 8; // Shift by a byte
    j--;
  }
  return bytearray;
}
