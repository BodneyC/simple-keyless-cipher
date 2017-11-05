#ifndef BENCRYPTION_H
#define BENCRYPTION_H

void power255(unsigned char* bytevals, int64_t &value);
unsigned char* workdown(int64_t value255, unsigned char *bytearray);
int bencrypt(std::ifstream& inputfile, std::fstream& encfile, std::string filename);
int dencrypt(std::ifstream& encfile, std::fstream& decfile, std::string filename);

#endif
