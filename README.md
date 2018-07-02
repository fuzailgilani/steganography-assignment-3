# Steganography Assignment 3 #

## Part 1: Calculate entropy of a file. ##

Completed. To compile code, run the following command in the directory that contains the .c file:

`gcc -o entropy assignment3part1.c -lm`

To calculate the entropy of a file named file.txt after compiling, use the following command:

`./entropy file.txt`

## Part 2: Hide a message in a text file, using LSB steganography. ##

Completed. To compile code, run the following command in the directory that contains the .c file:

`gcc -o hide assignment3part2.c -lm`

To hide a file named message.txt in a file name cover.txt, use the following command:

`./hide -h cover.txt message.txt`

The program will generate a file named stego.txt, which will contain the cover file's contents with the LSB of each character altered to hide the message file's contents. If stego.txt already exists, it will be over-written.

To extract a message hidden in stego.txt and save it to a file named hidden-message.txt, use the following command:

`./hide -e stego.txt hidden-message.txt`

The program will generate a file named hidden-message.txt, which will contain the message extracted from the LSB of each character in stego.txt. If hidden-message.txt already exists, it will be over-written.

The `-lm` flag is to link the mathematics library from C. This is required to use the math.h file in C.
