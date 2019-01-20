# Simple Keyless Cypher

This project is a C++ implementation of a simple cipher algorithm idea that I had a while back.

The repo recently underwent a significant overhall (since edc4e7f1b132cc055b04ebbdf38aa95a73fe0464) which took a purely procedural version of the project which created two binaries and wrote the whole program into a class (`GenEnc`).
## Size of Ciphered file

The nature of the algorithm slightly increases the size of the file when encrypted. There are eight bytes for a sort-of-header giving the size of the un-ciphered file, then the main content is size of the un-ciphered file, finally there is the size of the un-ciphered file over four, over eight (rounded up).

## Compilation

Clone the [repository](https://github.com/BodneyC/simple_keyless_cypher.git) and cd into the folder, then use `make` (GNU or Powershell should be fine):

```
git clone https://github.com/BodneyC/simple_keyless_cypher.git
cd simple_keyless_cypher/
make
```
## Usage

Usage:

	genenc [-[ed]] <input_file> [<output_file>]

Switches:

    -e  Cipher <input file> and write to <output file>
    -d  Un-cipher <input file> and write to <output file>

- If no switch is given, `-e` is assumed
- If no output file is specified, `a.out` is used
