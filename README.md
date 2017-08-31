# Keyless Cypher (Bencryption)

This project is a C++ implementation of a simple encryption algorithm idea that I had a while back; it is named "Bencryption" becuase just like its creator "Ben", it is poor. The reason that it should be considered simple is mainly in the fact that it is wholly keyless i.e. provides no further security beyond the inability to read the file as plain-text.

I realise that without a keystream it is essentially useless but it was a fun little project for learning C++ file IO. I have a couple of ideas to increase the security of the algorithm but they will have to wait for when I have the time.

TL,DR: Keyless cypher in a 5:4 compression ratio.

## Compilation

Clone the [repository](https://github.com/BodneyC/secdel.git) and cd into the folder:

```
git clone https://github.com/BodneyC/secdel.git`
cd secdel/
```
Then, if in **Windows**, just call `comp.bat`. Elsewise for **Linux**, the line `#include <stat.h>` needs to be replaced with 	`#include <sys/stat.h>` in each file; then, use the Makefile (`make`).

## Usage

The program takes file-paths at the command line and fills them with a random value, if "-r" is provided, the file is renamed also; both of these processes are done "-l" number of times (or once).

As the program creates a single byte array of the size of each file as default, the "-s" option can be used on larger files. The stream option ("-s") means that the program will at most use 1MiB for each file in turn for the deletion.

**Command format (TCLAP output):**

	USAGE:

	   main.exe  [-l <Integer>] [-r] [-s] [--] [--version] [-h] <String> ...


	Where:

	   -l <Integer>,  --levels <Integer>
	     Number of times to wipe

	   -r,  --rename
	     Rename [-l] levels before deletion

	   -s,  --stream
	     Wipe in small chunks

	   --,  --ignore_rest
	     Ignores the rest of the labeled arguments following this flag.

	   --version
	     Displays version information and exits.

	   -h,  --help
	     Displays usage information and exits.

	   <String>  (accepted multiple times)
	     (required)  Input File-path


	   Secure deletion tool for Windows
