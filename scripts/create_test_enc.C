#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

int main(int argc, char** argv)
{
	if(argc != 4) {
		std::cout << "Usage\n\t./create_test_enc string:file_out int:n_4 int:n_rem";
		return 1;
	}
	std::string file_out = argv[1];
	int n_4 = strtol(argv[2], nullptr, 10);
	int n_rem = strtol(argv[3], nullptr, 10);

	std::ofstream o_file(file_out.c_str(), std::ios::binary | std::ios::trunc);

	if(!o_file.good()) {
		std::cout << "Opening file \'" << file_out << "\' failed. Exiting...";
		return 1;
	}

	int32_t to_write_4 = 0xffffffff;
	int8_t to_write_rem = 0xFF;

	uint8_t bytes[4];
	for(int i = 0; i < n_4; i++) {
		int32_t tmp_to_write_4 = to_write_4;
		for(int j = sizeof(bytes) - 1; j >= 0; j--) {
			bytes[j] = tmp_to_write_4 & 0xFF;
			tmp_to_write_4 >>= 8;
		}
		o_file.write((char*) &bytes, sizeof(to_write_4));
	}
	for(int i = 0; i < n_rem; i++)
		o_file.write((char*) &to_write_rem, 1);

	return 0;
}
