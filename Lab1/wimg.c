#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

void printPixels(unsigned char *data, int low, int high, bool RGB) {
	int i=0,j = RGB ? 3 : 1;

	printf("%10s : %s\n", "Pixel", "Value");

	for (i = low*j; i < j * (low+high); i += j) {
		if (j==3)
			printf("%10d : %d %d %d\n", i/j, data[i], data[i+1], data[i+2]);
		else
			printf("%10d : %d\n", i, data[i]);
	}
}

void printErr(char *str, int err) {
	errno = err;
	char *msg  = " Usage: wimg {options}\n\n Options:\n    Specify file\n\t-f {FILENAME}\n ";
	char *pstr = (char *)malloc(strlen(str) + strlen(msg));
	strcat(pstr, msg);
	strcat(pstr, str);
	perror(pstr);
	exit(0);
}

int main (int argc, char *argv[]) {
	FILE			*fp;
	int 			i,j,k,rows,cols;
	unsigned char	header[80];

	header[0] 		= '\0';

	if (argc <= 2) {
		printErr("No file", ENOENT);
	}

	for (i = 0; i < argc-1; i++) {
		if (argc > i+1 && strcmp(argv[i], "-f") == 0) { // flag == "-f"
			if (strcmp(&argv[i+1][strlen(argv[i+1]) - 4], ".ppm") != 0) {
				printErr(argv[i+1],EINVAL);
			} else {  // open "-f {FILENAME}"
				fp = fopen(argv[i+1],"rb");

				if (fp == NULL) {
					printErr(argv[i+1], ENOENT);
				}

				i++; // account for filename
			}
		} else if (argc > i+2 && strcmp(argv[i], "-p") == 0) {
			rows = atoi(argv[i+1]);
			cols = atoi(argv[i+2]);
		} else if (i > 0){
			printErr(argv[i], EINVAL);
		}
	}

	fopen("images/mod.ppm","wb");

	strcpy((char *)header,"P6 100 100 255 ");

	fprintf(fp,"%s",header);

	for (rows = 0; rows < 100; rows++) {
		for (cols = 0; cols < 100; cols++) {
			if (rows < 33) {
				i = 255;
				j = 0;
				k = 0;
			} else if (rows < 66) {
				i = 0;
				j = 255;
				k = 0;
			} else {
				i = 0;
				j = 0;
				k = 255;
			}
			fputc(i,fp);
			fputc(j,fp);
			fputc(k,fp);
		}
	}

	return 0;
}