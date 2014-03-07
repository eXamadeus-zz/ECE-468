// LATER
#include "../headers/ex_huffenc.h"
#include "../headers/ex_huffdec.h"
// #include "../headers/ex_bintree.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																													//
//	FILE SETUP :																									//
//																													//
//		INT num_symbols_in_tree  ||  INT num_encoded | INT ARRAY[][] (1: symbol, 2: frequency) || ENCODED DATA		//
//																													//
//																													//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

typedef struct _sym_table {
	int				symbol, weight, length;
	unsigned int	pattern;
} SYM_TABLE;

void printErr(char *, int);

int
main (int argc, char *argv[])
{  // return 0 on success, 1 on fail
	int		i;
	char	*fileName, *huffName;
	FILE	*inputFile, *huffFile, *outputFile;
	bool	COMPRESS;

	//////////////////////////
	/// SET DEFAULT VALUES ///
	//////////////////////////

	COMPRESS = true;

	//////////////////////////
	/// VERIFY/PARSE INPUT ///
	//////////////////////////

	if (argc < 2)
		printErr((char *)"No file", ENOENT);

	for (i = 2; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
			COMPRESS = false;
		else
			printErr(argv[i], EINVAL);
	}

	/* /////////////|\\\\\\\\\\\\\\\ */
	/* ||| COMPRESS | DECOMPRESS ||| */
	/* \\\\\\\\\\\\\|/////////////// */

	inputFile = fopen(argv[1],"rb");
	if (inputFile == NULL)
		printErr((char *)argv[1], ENOENT);
	
	fileName = argv[1]; // flag for later

	if (!COMPRESS)
	{
		strcat(fileName, "dec");
		outputFile = fopen(fileName, "wb");
	}

	if (COMPRESS)
	{
		huffName = (char *) malloc (sizeof(char) * strlen(fileName) + 4);
		strcpy(huffName, fileName);
		strcat(huffName, ".huf");
		huffFile = fopen(huffName, "ab+");
		// printf("\nin: %s  huf: %s\n", fileName, huffName);
		huffencode(inputFile, huffFile);
	}
	else
	{
		rewind(inputFile);
		// printf("\nhuf: %s  out: %s\n", huffName, fileName);
		huffdecode(inputFile, outputFile);
	}

	// Waiting on James' code implementation.
	//COMPRESS ? EX_RLE_COMPRESS(inputFile,outputFile) : EX_RLE_DECOMPRESS(inputFile,outputFile);

	//////////////////////
	/// END STATEMENTS ///
	//////////////////////

	fclose(inputFile);
	
	if (!COMPRESS)
	{
		fclose(outputFile);
	}
	else
	{
		free(huffName);
		fclose(huffFile);
	}

	exit(0);
}

//function definitions

void
printErr(char *str, int err)
{
	errno      = err;
	char *msg  = (char *)"Error:\n    ";
	char *pstr = NULL;
	pstr       = (char *)malloc(strlen(str) + strlen(msg));

	strcat(pstr, msg);
	strcat(pstr, str);
	perror(pstr);

	exit(err);
}