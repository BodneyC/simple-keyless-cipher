#include "GenEnc.H"

GenEnc::GenEnc(CMDArgs& cmd_args) 
{
	file_in = cmd_args.files[0];
	file_out = cmd_args.files[1];

	int acc_255 = 1, acc_256 = 1;

	for(int i = 0; i < sizeof(pow_255); i++) {
		pow_255[i] = acc_255;
		acc_255 *= 255ll;
		pow_256[i] = acc_256;
		acc_256 *= 256ll;
	}
}

GenEnc::~GenEnc() 
{
	if(i_file.is_open())
		i_file.close();
	if(o_file.is_open())
		o_file.close();
}

int GenEnc::open_files()
{
	std::cout << "In open_files..." << std::endl;
	i_file.open(file_in.c_str(), std::ios::binary);
	if(!i_file.good())
		return FILE_IO_ERROR;
	std::cout << "Opened i_file successfully..." << std::endl;

    i_file.seekg(0, i_file.end);
    i_file_size = i_file.tellg();
    i_file.seekg(0, i_file.beg);

	std::cout << "Got i_file_size..." << std::endl;

	o_file.open(file_out.c_str(), std::ios::binary | std::ios::trunc);

	if(!o_file.good())
		return FILE_IO_ERROR;
	
	return 0;
}

int GenEnc::decrypt()
{

}

int GenEnc::encrypt()
{
	_write_size_little_endian();

	iterations = i_file_size / PRE_CHUNK_SIZE;

	for(int64_t i = 0; i < iterations; i++)
		_write_chunk();

	// Fill the remaining five
	// NOTE: Likely to segfault on first try
	if(i_file_size % PRE_CHUNK_SIZE) {
		for(int i = 0; i < sizeof(byte_vals_255); i++) {
			int8_t tmp_char;
			if(!i_file.eof())
				tmp_char = i_file.get();
			if(i_file.eof())
				byte_vals_255[i] = 0;
			else
				byte_vals_255[i] = tmp_char;
		}

		// Remainder
		o_file.write((char*) &byte_vals_255, sizeof(byte_vals_255));
	}

	// Bit array
	o_file.write((char*) &bit_vector[0], ceil(iterations / 8));

	return 0;
}

void GenEnc::_write_size_little_endian()
{
	int8_t bytes[8];
	for(int i = 7; i >= 0; i--) {
		bytes[i] = i_file_size & 0xFF;
		i_file_size >>= 8;
	}
	o_file.write((char*) &bytes, sizeof(int64_t));
}

void GenEnc::_write_chunk()
{
	i_file.read((char*) &byte_vals, PRE_CHUNK_SIZE);
	int64_t dec_value = _bytes_to_dec(/* 4, byte_vals */);
	_enc_workdown(dec_value);
	o_file.write((char*) &byte_vals_255, sizeof(byte_vals_255));
}

void GenEnc::_enc_workdown(int64_t dec_value)
{
	// Highest byte is [01]
	if(dec_value > pow_255[4]) {
		bit_vector.push_back(1);
		dec_value -= pow_255[4];
	} else {
		bit_vector.push_back(0);
	}

	int j;
	for(int i = 3; i > 0; i--) {
		j = 254;
		int acc = j * pow_255[i];
		while(dec_value - acc < 0)
			j--;
		// NOTE: Likely to segfault on first try
		byte_vals_255[i] = j;
		dec_value -= (j * pow_255[i]);
	}

	byte_vals_255[0] = dec_value;
}

int64_t GenEnc::_bytes_to_dec(int n_bytes)
{
	int64_t ret_val = 0;
	int j = n_bytes - 1;

	if(n_bytes == sizeof(int32_t))
		for(int i = 0; i < n_bytes; i++, j--) 
			ret_val += byte_vals[j] * pow_256[i];
	else
		for(int i = 0; i < n_bytes; i++, j--) 
			ret_val += val_64_bit[j] * pow_256[i];

	return ret_val;
}

void GenEnc::_dec_to_bytes(int64_t value, int n_bytes)
{
    int j = n_bytes - 1;

    for (int i = 0; i < n_bytes; i++, j--) {
        val_64_bit[j] = value & 0xFF;
        value = value >> 8;
    }
}