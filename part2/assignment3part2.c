#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// function prototypes
void printHelpMessage();
void int_to_bin_digit(unsigned int in, int count, int* out);
int get_file_length(FILE *fp);
void print_binary_array(int * binary);
int bin_digit_to_int(int * binary, int numOfDigits);

int main(int argc, char * argv[]) {

	if (argc != 4){
		// if argument count isn't four, print help message
		printHelpMessage();
		return 0;
	}

	if(strcmp(argv[1], "-h") == 0){
		// open cover file - binary read
		FILE * cover_fp = fopen(argv[2], "rb");
		if(cover_fp == NULL){
			perror("Cover file could not be opened");
			return 1;
		}

		// open message file - binary read
		FILE * message_fp = fopen(argv[3], "rb");
		if(message_fp == NULL){
			perror("Message file could not be opened");
			return 1;
		}

		// open output file - binary write
		FILE * stego_fp = fopen("stego.txt", "wb");
		if(stego_fp == NULL) {
			perror("Stego file could not be opened");
			return 1;
		}

		// variable to see if full message will be hidden
		int full_message_hidden = 1;

		// see how long cover and message files are
		int cover_length = get_file_length(cover_fp);
		int message_length = get_file_length(message_fp); // length of message in file
		printf("Message length: %d\n", message_length);
		int bytes_for_length = sizeof(message_length);
		int hidden_message_length = message_length + bytes_for_length + 1; // length of message once it is hidden; incremented by 1 to make room for message length

		// if message file has more bits than length of cover file, then full message will not be hidden
		if (cover_length < message_length * 8) {
			full_message_hidden = 0;
		}

		// extract binary representation of cover file
		int cover_in_binary[cover_length][8];
		int c;
		int count = 0;
		while(1) {
			c = fgetc(cover_fp);
			if( feof(cover_fp) ) {
				 break;
			}
			int_to_bin_digit(c, 8, cover_in_binary[count]);
			count++;
		}

		// extract binary representation of message file
		int message_in_binary[hidden_message_length][8];

		int_to_bin_digit(bytes_for_length, 8, message_in_binary[0]);
		for (int i = bytes_for_length - 1; i > -1; i--) {
			int part_of_message_length = (message_length >> (8*i)) & 0xff;
			int_to_bin_digit(part_of_message_length, 8, message_in_binary[bytes_for_length - i]);
		}

		count = bytes_for_length + 1;
		while(1) {
			c = fgetc(message_fp);
			if( feof(message_fp) ) {
				 break;
			}
			int_to_bin_digit(c, 8, message_in_binary[count]);
			count++;
		}

		// hiding message
		if (full_message_hidden == 0) {
			printf("Not all of the message will be hidden, because the cover file is too small.\n");
			for(int i = 0; i < cover_length; i++) {
				int current_bit_to_hide = message_in_binary[i / 8][i % 8];
				cover_in_binary[i][7] = current_bit_to_hide;
			}
		} else {
			for(int i = 0; i < hidden_message_length * 8; i++) {
				int current_bit_to_hide = message_in_binary[i / 8][i % 8];
				cover_in_binary[i][7] = current_bit_to_hide;
			}
		}

		// converting modified cover_in_binary into a string
		char stego_string[cover_length + 1];
		stego_string[cover_length] = '\0';

		for(int i = 0; i < cover_length; i++) {
			stego_string[i] = (char)bin_digit_to_int(cover_in_binary[i], 8);
		}

		// writing stego_string to output file
		int bytesWritten = fwrite(stego_string, 1, (sizeof(stego_string) - 1), stego_fp);

		// closing files
		fclose(cover_fp);
		fclose(message_fp);
		fclose(stego_fp);

	} else if (strcmp(argv[1], "-e") == 0){
		// open stego file
		FILE * stego_fp = fopen(argv[2], "rb");
		if (stego_fp == NULL) {
			perror("Stego file could not be opened");
			return 1;
		}
		// open output file
		FILE * output_fp = fopen(argv[3], "wb");
		if (output_fp == NULL) {
			perror("Output file could not be opened");
			return 1;
		}

		// read stego text from file
		int stego_length = get_file_length(stego_fp);
		char * stego_string = malloc((stego_length+1));
		if (stego_string == NULL){
			perror("Stego string could not be allocated in memory");
			return 1;
		}
		int bytesRead = fread(stego_string, 1, stego_length, stego_fp);


		// extract first byte from stego_string; this is the number of bytes used to store the length of the message.
		int message_length_bytes;

		for(int i = 0; i < 8; i++) {
			int first_byte = (int)stego_string[i];
			int first_byte_binary[8];
			int message_length_binary[8];
			int_to_bin_digit(first_byte, 8, first_byte_binary);
			message_length_binary[i] = first_byte_binary[7];
			if(i == 7){
				message_length_bytes = bin_digit_to_int(message_length_binary, 8);
			}
		}

		// extract a number of bytes equivalent to message_length_bytes; this is the length of the message
		int message_length;

		for(int i = 8; i < 8 + message_length_bytes * 8; i++) {
			int first_byte = (int)stego_string[i];
			int first_byte_binary[8];
			int message_length_binary[message_length_bytes * 8];
			int_to_bin_digit(first_byte, 8, first_byte_binary);
			message_length_binary[i-8] = first_byte_binary[7];
			if(i == 7 + message_length_bytes * 8){
				message_length = bin_digit_to_int(message_length_binary, message_length_bytes * 8);
			}
		}

		printf("Extracted message length: %d\n", message_length);

		// create string to contain extracted message
		char extracted_string[message_length + 1];
		extracted_string[message_length] = '\0';

		// extract remaining bytes, bit by bit, and construct extracted_string
		int bit_in_current_byte = 0;
		int char_counter = 0;
		for(int i = 8 + message_length_bytes * 8; i < (message_length + message_length_bytes + 1) * 8; i++){
			int byte_from_string = (int)stego_string[i];
			int bye_from_string_binary[8];
			int current_character_binary[8];
			int_to_bin_digit(byte_from_string, 8, bye_from_string_binary);
			current_character_binary[bit_in_current_byte] = bye_from_string_binary[7];
			bit_in_current_byte++;
			if( ((i+1) % 8) == 0){
				int current_character = bin_digit_to_int(current_character_binary, 8);
				bit_in_current_byte = 0;
				extracted_string[char_counter] = (char) current_character;
				char_counter++;
			}
		}

		// save extracted string to output file.
		int bytesWritten = fwrite(extracted_string, 1, (sizeof(extracted_string) - 1), output_fp);

		// close files
		fclose(stego_fp);
		fclose(output_fp);
	} else {
		// if flag is unrecognized, indicate flag is unrecognized and print help message
		printf("%s is an invalid flag.\n\n\n", argv[1]);
		printHelpMessage();
		return 0;
	}

	return 0;
}

/*
** void int_to_bin_digit(unsigned int in, int count, int* out)
** Converts integer into binary number stored in array
** Parameters:
** 	in - unsigned int - integer to be converted to binary
**  count - int - digits of binary number desired
**  out - int * - array binary number is stored in
** Returns:
**	void - binary array returned through parameter
*/
void int_to_bin_digit(unsigned int in, int count, int* out) {
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

/*
** int bin_digit_to_int(int * binary, int numOfDigits)
** Converts binary number stored in array into integer
** Parameters:
** 	binary - int * - array containing binary number
**  numOfDigits - int - number of digits binary number contains
** Returns:
**	res - int - converted integer
*/
int bin_digit_to_int(int * binary, int numOfDigits){
	int res = 0;
	for(int i = 0; i < numOfDigits; i++){
		if (binary[i] == 1) {
			res += pow(2, numOfDigits-1-i);
		}
	}
	return res;
}

/*
** int get_file_length(FILE *fp)
** Counts number of characters in file
** Parameters:
** 	fp - FILE * - pointer to file that characters are being counted from
** Returns:
**	size - int - count of characters in file
*/
int get_file_length(FILE *fp){
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

/*
** void print_binary_array(int * binary)
** Prints 8 digit binary number stored in array
** Parameters:
** 	binary - int * - pointer to array containing digits of 8 digit binary number
** Returns:
**	void
** Assumes:
**	that binary is only 8 digits.
*/
void print_binary_array(int * binary){
	for(int i = 0; i < 8; i++) {
		printf("%d", binary[i]);
	}
}

/*
** void printHelpMessage()
** Prints instructions for how to use file.
** Parameters:
** 	none
** Returns:
**	void
*/
void printHelpMessage() {
	printf("To hide a message, use the following syntax, replacing the file names as needed.\nThe output will be stego.txt, which will contain the cover file with the LSB altered to hide the message file.\nIf stego.txt already exists, it will be overwritten.\n\n");
	printf("./hide -h cover_file.txt message_file.txt\n\n\n");

	printf("To extract a message, use the following syntax, replacing the file names as needed.\n The output will be stored in the second argument provided after the -e flag, and will contain the extracted message from the stego.txt file.\nIf message_file.txt already exists, it will be overwritten.\n\n");
	printf("./hide -e stego.txt message_file.txt\n\n\n");
}
