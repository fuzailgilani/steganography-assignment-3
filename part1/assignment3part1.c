#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

	FILE * fp = fopen(argv[1], "r");
	int c;

	while(1) {
      c = fgetc(fp);
      if( feof(fp) ) {
         break ;
      }
      printf("%d\n", c);
  }

	fclose(fp);

	return 0;
}
