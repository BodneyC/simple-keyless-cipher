#include "GenEnc.H"

int process_args(int argc, char** argv, CMDArgs& cmd_args)
{
	int ind = 0;
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][1] == 'e') {
				if(cmd_args.enc_dec == 1)
					return USAGE_ERROR;
				cmd_args.enc_dec = 0;
				continue;
			}
			if(argv[i][1] == 'd') {
				if(cmd_args.enc_dec == 0)
					return USAGE_ERROR;
				cmd_args.enc_dec = 1;
				continue;
			}
			return USAGE_ERROR;
		}
		cmd_args.files[ind] = argv[i];
		ind++;
	}
	if(cmd_args.files[0] == "") 
		return USAGE_ERROR;
	return 0;
}

void show_help() 
{
	std::cout << "Usage: program [-[ed]] <input_file> [<output_file>]" << std::endl;
}

int main(int argc, char** argv)
{
	CMDArgs cmd_args;
	if(process_args(argc, argv, cmd_args)) {
		show_help(); 
		return USAGE_ERROR;
	}

	GenEnc gen_enc(cmd_args);
	std::cout << "Initialised gen_enc successfully..." << std::endl;

	if(gen_enc.open_files())
		return FILE_IO_ERROR;
	std::cout << "Files opened successfully..." << std::endl;

	// if(cmd_args.enc_dec) {
	// 	gen_enc.decrypt();
	// } else {
	// 	gen_enc.encrypt();
	// }

	return 0;
}
