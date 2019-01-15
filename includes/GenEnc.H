#include <iostream>
#include <string>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <deque>
#include <cmath>

#define USAGE_ERROR -1
#define PROG_FAIL -2
#define FILE_IO_ERROR -3

#define PRE_CHUNK_SIZE 4
#define PRO_CHUNK_SIZE 5

typedef struct CMDArgs {
	std::string files[2];
	int enc_dec;

	CMDArgs(): files{ "", "a.enc" }, enc_dec(-1) {}
} CMDArgs;

class GenEnc {
private:
	std::deque< bool > bit_vector;
	uint8_t byte_vals[4], byte_vals_255[4], val_64_bit[8];
	int64_t i_file_size, o_file_size;
	int64_t iterations;
	int64_t pow_255[5], pow_256[5];
	std::ifstream i_file;
	std::ofstream o_file;

	// Enc Helpers
	void _dec_to_bytes(int64_t value, int n_bytes = sizeof(int32_t));
	int64_t _bytes_to_dec(int n_bytes = sizeof(int32_t));
	void _enc_workdown(int64_t dec_value);
	void _write_chunk();
	void _write_size_little_endian();

	// Dec helpers
	void _calc_o_file_size() ;

public:
	std::string file_in, file_out;

	GenEnc(CMDArgs& cmd_args);
	~GenEnc();
	
	int open_files();
	int encrypt();
	int decrypt();
};