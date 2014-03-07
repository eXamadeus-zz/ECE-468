#include "ex_rle.h"
// #include "ex_dict.h"

#define MARKER 0
#define MAX_RLE	255

typedef unsigned char BYTE;

// INTERNAL FUNCTION PROTOTYPES

void __write_run(BYTE, BYTE, FILE *);
void __write_buff(BYTE *, BYTE, FILE *);

// EXTERNAL FUNCTION DEFINITIONS

void EX_RLE_COMPRESS(FILE *in_fp, FILE *out_fp) {
	BYTE	runCount, prev, curr;	
	bool	readTrue;

	runCount = 1;
	prev     = 0;
	curr     = 0;

	readTrue = fread(&prev,1,1,in_fp);

	while (readTrue) {
		readTrue = fread(&curr,1,1,in_fp);
		if (!readTrue) {
			__write_run(runCount,prev,out_fp);
			break;
		}

		if (curr == prev) {
			if (runCount == MAX_RLE) {
				__write_run(runCount,prev,out_fp);
				prev     = curr;
				runCount = 1;
			} else runCount++;
		} else {
			__write_run(runCount,prev,out_fp);
			prev     = curr;
			runCount = 1;
		}
	}
}

void EX_RLE_DECOMPRESS(FILE *in_fp, FILE *out_fp) {
	BYTE	count, symbol;
	bool	readTrue;
	int		i;

	i        = 0;
	count    = 0;
	readTrue = true;

	while (readTrue) {
		readTrue = fread(&count,1,1,in_fp);
		if (!readTrue)
			break;

		fread(&symbol,1,1,in_fp);

		for (i=0; i<count; i++)
			fwrite(&symbol,1,1,out_fp);
	}

	// if(!symbol)
	// 	fwrite(&symbol,1,1,out_fp);
}

// INTERNAL FUNCTION DEFINITIONS

void __write_run(BYTE count, BYTE A, FILE *out) {
	fwrite(&count,1,1,out);
	fwrite(&A,1,1,out);
}
