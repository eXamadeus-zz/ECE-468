#include "../headers/ex_rle.h"
#include "../headers/ex_lzw.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define RLE_ENC 0
#define LZW_ENC 1

//Usage: printErr(Message, Errno);
void printErr(char *, int);

int main (int argc, char *argv[]) {  // return 0 on success, 1 on fail

	int		ENC_TYPE = RLE_ENC,i;
	char	*fileName;
	FILE 	*inputFile,*outputFile;
	bool	COMPRESS;

	//////////////////////////
	/// SET DEFAULT VALUES ///
	//////////////////////////

	ENC_TYPE = RLE_ENC;
	COMPRESS = true;

	//////////////////////////
	/// VERIFY/PARSE INPUT ///
	//////////////////////////

	if (argc < 2) {
		printErr((char *)"No file", ENOENT);
	}

	inputFile = fopen(argv[1],"rb");
	fileName = argv[1]; // flag for later

	if (inputFile == NULL) {
		printErr((char *)argv[1], ENOENT);
	}

	for (i = 2; i < argc; i++) {
		if (strcmp(argv[i], "-l") == 0) {
			ENC_TYPE = LZW_ENC;
		} else if (strcmp(argv[i], "-d") == 0) {
			COMPRESS = false;
		} else {
			printErr(argv[i], EINVAL);
		}
	}

	/* /////////////|\\\\\\\\\\\\\\\ */
	/* ||| COMPRESS | DECOMPRESS ||| */
	/* \\\\\\\\\\\\\|/////////////// */

	switch (ENC_TYPE) {
		case RLE_ENC:
			if (COMPRESS) strcat(fileName, ".rle");
			else {
				char ext[10];
				strcpy(ext,&fileName[strlen(fileName)-8]);
				ext[4] = '\0';
				if (ext[0] == '.') fileName[strlen(fileName)-8] ='\0';
				else fileName[strlen(fileName)-4] ='\0';
				strcat(fileName, "dec");
				if (ext[0] == '.') strcat(fileName, ext);
			}
			outputFile = fopen(fileName,"wb");
			COMPRESS ? EX_RLE_COMPRESS(inputFile,outputFile) : EX_RLE_DECOMPRESS(inputFile,outputFile);
			break;
		case LZW_ENC:
			if (COMPRESS) strcat(fileName, ".lzw");
			else {
				char ext[10];
				strcpy(ext,&fileName[strlen(fileName)-8]);
				ext[4] = '\0';
				if (ext[0] == '.') fileName[strlen(fileName)-8] ='\0';
				else fileName[strlen(fileName)-4] ='\0';
				strcat(fileName, "dec");
				if (ext[0] == '.') strcat(fileName, ext);
			}
			outputFile = fopen(fileName,"wb");
			COMPRESS ? EX_LZW_COMPRESS(inputFile,outputFile) : EX_LZW_DECOMPRESS(inputFile,outputFile);
			break;
		default:
			exit(100);
	}

	//////////////////////
	/// END STATEMENTS ///
	//////////////////////

	fclose(inputFile);
	fclose(outputFile);

	exit(0);
}

//function definitions

void printErr(char *str, int err) {
	errno = err;
	char *msg  = (char *)" Usage: lab2 {fileName} {options}\n\nOptions:\n    Decompress\n\t-d\n    -l\n\t-Use LZW (Default RLE)\n\nError:\n    ";
	char *pstr = NULL;
	pstr = (char *)malloc(strlen(str) + strlen(msg));
	strcat(pstr, msg);
	strcat(pstr, str);
	perror(pstr);
	exit(err);
}