#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

int main(int argc, char** argv)
{
	std::string file_out = "a.out";

	if(argc == 2)
		file_out = argv[1];

	std::ofstream o_file(file_out.c_str(), std::ios::binary | std::ios::trunc);

	if(!o_file.good()) {
		std::cout << "Opening file \'" << file_out << "\' failed. Exiting...";
		return 1;
	}

	int64_t to_write = 2;
	uint8_t bytes[8];

	for(int i = 7; i >= 0; i--) {
		bytes[i] = to_write & 0xFF;
		to_write >>= 8;

	}

	std::cout << sizeof(int8_t) << std::endl;
	std::cout << sizeof(char) << std::endl;

	o_file.write((char*) &bytes, sizeof(int64_t));

	return 0;
}
