#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double log_base_2(double num); // function prototype for log_base_2() function
void printHelpMessage();

int main(int argc, char * argv[]) {

	if(argc != 2){ // if incorrect number of arguments for usage, print help message and quit execution
		printHelpMessage();
		return 0;
	}

	FILE * fp = fopen(argv[1], "rb"); // open the file in binary read mode

	if (fp == NULL) { // error handling for file open
		perror("File could not be opened");
		return 1;
	}

	int c; // variable used to store each character as it is read from file
	int sumOfUniqueChar = 0; // variable to store sum of the total unique characters in the file
	int lengthOfFile = 0; // variable to store total number of characters the file contains
	int asciiArr[256] = {0}; // array where each index corresponds to an ASCII code for a character. The element stored in each index is the number of times that character appears in the file
	double entropy = 0; // variable to store entropy as it is calculated
	double ratio = 0; // variable to store ratio of actual entropy to maximum entropy as it is calculated

	while(1) { // keep looping
      c = fgetc(fp); // retrieve on character at a time from file
      if( feof(fp) ) { // if end of file,
         break; //        	break loop
      }

			asciiArr[c]++; // increment element of array where index == ASCII representation of current character
			lengthOfFile++; // increment counter variable that tracks number of characters in file
			if(asciiArr[c] == 1) { // if current character has been encountered for the first time
				sumOfUniqueChar++; //			increment counter variable that tracks number of unique characters in file
			}
  }

	printf("Total number of unique characters: %d\n", sumOfUniqueChar); // print number of unique characters

	for(int i = 0; i < 256; i++){ // loop through array containing total of each ASCII character
		double probabilityOfChar = (double)asciiArr[i] / (double)lengthOfFile; // calculate probability of character within string; total times it appears divided by total length of file

		if (asciiArr[i] != 0) { // if the character appears in the file,
			entropy += probabilityOfChar * log_base_2(1 / probabilityOfChar); // then calculate the entropy it contributes, and increment total entropy by that amount
		}
	}

	ratio = entropy / 8; // calculate ratio of actual entropy to maximum; 256 possible characters means there is a maximum of 8 bits of entropy

	printf("Entropy: %lf\n", entropy); // display entropy and ratio
	printf("Ratio: %lf\n", ratio);

	fclose(fp); // close file

	return 0; // end function
}


/*
** double log_base_2(double num)
** Calculates log base 2 of argument
** Parameters:
** 	num - double - number to calculate log base 2 of
** Returns:
**  log(num) / log(2) - calculates log base 2 using log change of base formula
*/
double log_base_2(double num) {
	return log(num) / log(2);
}

/*
** void printHelpMessage()
** Prints instructions for how to use file.
** Parameters:
** 	none
** Returns:
**	void
*/
void printHelpMessage(){
	printf("To calculate the entropy of a file, use the following syntax, replacing the file name as needed.\nThe output will be printed to the screen, showing the number of unique characters, the entropy, and the ratio of entropy to greatest possible entropy.\n\n");
	printf("./entropy file_name.txt\n\n\n");
}
