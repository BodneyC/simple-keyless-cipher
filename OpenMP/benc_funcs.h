#ifndef BENCRYPTION_H
#define BENCRYPTION_H

void power255(unsigned char* bytevals, int64_t &value);
unsigned char* workdown(int64_t value255, unsigned char *bytearray);
int bencrypt(std::string inputFile, std::string outputFile);
int dencrypt(std::string encfileString, std::string decfileString);

#endif
