#include "GenEnc.H"

GenEnc::GenEnc(CMDArgs& cmd_args): o_file_size(0)
{
	file_in = cmd_args.files[0];
	file_out = cmd_args.files[1];

	uint64_t acc_255 = 1, acc_256 = 1;

	for(int i = 0; i < PRO_CHUNK_SIZE; i++) {
		pow_255[i] = acc_255;
		acc_255 *= 255ll;
		pow_256[i] = pow_val_64_bit[i] = acc_256;
		acc_256 *= 256ll;
	}
	for(int i = PRO_CHUNK_SIZE; i < 8; i++) {
		pow_val_64_bit[i] = acc_256;
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
	i_file.open(file_in.c_str(), std::ios::binary);
	if(!i_file.good())
		return FILE_IO_ERROR;

    i_file.seekg(0, i_file.end);
    i_file_size = i_file.tellg();
    i_file.seekg(0, i_file.beg);

	o_file.open(file_out.c_str(), std::ios::binary | std::ios::trunc);

	if(!o_file.good())
		return FILE_IO_ERROR;
	
	return 0;
}

int GenEnc::decrypt()
{
	_calc_o_file_size();

	bit_arr_offset = 8 + o_file_size + (o_file_size % PRE_CHUNK_SIZE);
	bit_arr = new int8_t[i_file_size - bit_arr_offset]();

	_read_bit_arr_from_file();

	for(int i = 0; i < sizeof(bit_arr); i++)
	std::cout << (unsigned)bit_arr[i] << std::endl;

	iterations = o_file_size / PRE_CHUNK_SIZE;

	for(int64_t i = 0; i < iterations; i++) 
		_dec_write_chunk(i);

	// NOTE: Remainder is written as four bytes, and so will give a spurious 4 - remainder x00s
	if(o_file_size % PRE_CHUNK_SIZE) {
		_dec_write_chunk(iterations);
	}

	delete[] bit_arr;

	return 0;
}

void GenEnc::_dec_write_chunk(int64_t iter)
{
	i_file.read((char*) byte_vals_255, PRE_CHUNK_SIZE);
	int64_t dec_value = _bytes_to_dec(255);
	if(bit_arr[iter / 8] >> (iter % 8) & 1)
		dec_value += pow_255[4];
	_dec_to_bytes(dec_value, 4);
	if(iter != iterations)
		o_file.write((char*) &byte_vals, sizeof(byte_vals));
	else
		o_file.write((char*) &byte_vals, o_file_size % PRE_CHUNK_SIZE);
}

void GenEnc::_read_bit_arr_from_file()
{
	int64_t curr_offset = i_file.tellg();
	i_file.seekg(bit_arr_offset, i_file.beg);
	i_file.read((char*) bit_arr, i_file_size - bit_arr_offset); 
	i_file.seekg(curr_offset, i_file.beg);
}

void GenEnc::_calc_o_file_size() 
{
	i_file.read((char*) &val_64_bit, sizeof(o_file_size));

	for(int i = 0, j = 7; i < 8; i++, j--)
		o_file_size += val_64_bit[j] * pow_val_64_bit[i];
}

int GenEnc::encrypt()
{
	int64_t n_byte_bit_arr = ceil(ceil((double) i_file_size / 4) / 8);
	bit_arr = new int8_t[n_byte_bit_arr]();
	iterations = i_file_size / PRE_CHUNK_SIZE;

	std::cout << iterations << " iterations required..." << std::endl <<
		i_file_size % PRE_CHUNK_SIZE << " remainder..." << std::endl;

	// Write filesize
	_dec_to_bytes(i_file_size, 8);
 	o_file.write((char*) &val_64_bit, sizeof(val_64_bit));
	std::cout << "Written filesize (little endian)..." << std::endl;

	for(int64_t i = 0; i < iterations; i++)
		_enc_write_chunk(i);

	if(i_file_size % PRE_CHUNK_SIZE)
		_enc_write_chunk_remainder();

	o_file.write((char*) bit_arr, n_byte_bit_arr);

	delete[] bit_arr;

	return 0;
}

void GenEnc::_enc_write_chunk_remainder()
{
	for(int i = 0; i < PRE_CHUNK_SIZE; i++) {
		int8_t tmp_char;
		if(!i_file.eof())
			tmp_char = i_file.get();
		if(i_file.eof())
			byte_vals_255[i] = 0;
		else
			byte_vals_255[i] = tmp_char;
	}
	int64_t dec_value = _bytes_to_dec(256);
	if(dec_value > pow_255[4]) {
		bit_arr[iterations / 8] += pow(2, iterations % 8);
		dec_value -= pow_255[4];
	}
	_enc_workdown(dec_value);
	o_file.write((char*) &byte_vals_255, PRE_CHUNK_SIZE);
}

void GenEnc::_enc_write_chunk(int64_t iter)
{
	i_file.read((char*) byte_vals, PRE_CHUNK_SIZE);
	int64_t dec_value = _bytes_to_dec(256);
	if(dec_value > pow_255[4]) {
		bit_arr[iter / 8] += pow(2, iter % 8);
		dec_value -= pow_255[4];
	}
	_enc_workdown(dec_value);
	o_file.write((char*) &byte_vals_255, PRE_CHUNK_SIZE);
}

void GenEnc::_enc_workdown(int64_t dec_value)
{
	// Highest byte is [01]
	for(int i = 3; i > 0; i--) {
		int64_t j = 254;
		while(dec_value - (j * pow_255[i]) < 0)
			j--;
		// NOTE: Likely to segfault on first try
		byte_vals_255[i] = j;
		dec_value -= (j * pow_255[i]);
	}

	byte_vals_255[0] = dec_value;
}

int64_t GenEnc::_bytes_to_dec(int pow_num)
{
	int64_t ret_val = 0;
	int j = PRE_CHUNK_SIZE - 1;

	if(pow_num == 256)
		for(int i = 0; i < PRE_CHUNK_SIZE; i++, j--) 
			ret_val += byte_vals[j] * pow_256[i];
	else
		for(int i = 0; i < PRE_CHUNK_SIZE; i++, j--) 
			ret_val += byte_vals_255[i] * pow_255[i];

	return ret_val;
}

void GenEnc::_dec_to_bytes(int64_t value, int n_bytes)
{
    int j = n_bytes - 1;

	if(n_bytes == sizeof(int64_t)) {
	    for (int i = 0; i < n_bytes; i++, j--) {
	        val_64_bit[j] = value & 0xFF;
	        value >>= 8;
	    }
	} else {
	    for (int i = 0; i < n_bytes; i++, j--) {
	        byte_vals[j] = value & 0xFF;
	        value >>= 8;
	    }
	}
}
