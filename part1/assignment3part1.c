#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double log_base_2(double num);

int main(int argc, char * argv[]) {

	FILE * fp = fopen(argv[1], "r");
	int c;
	int sumOfUniqueChar = 0;
	int totalNumOfCharacters = 0;
	int asciiArr[256] = {0};
	double entropy = 0;

	while(1) {
      c = fgetc(fp);
      if( feof(fp) ) {
         break;
      }

			asciiArr[c]++;
			totalNumOfCharacters++;
			if(asciiArr[c] == 1) {
				sumOfUniqueChar++;
			}
  }

	printf("Total number of unique characters: %d\n", sumOfUniqueChar);

	for(int i = 0; i < 256; i++){
		double probabilityOfChar = (double)asciiArr[i] / (double)totalNumOfCharacters;

		if (asciiArr[i] != 0) {
			entropy += probabilityOfChar * log_base_2(1 / probabilityOfChar);
		}
	}

	double ratio = entropy / 8;

	printf("Entropy: %lf\n", entropy);
	printf("Ratio: %lf\n", ratio);

	fclose(fp);

	return 0;
}

double log_base_2(double num) {
	return log(num) / log(2);
}
