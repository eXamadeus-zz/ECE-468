#include "ex_lzw.h"
#include "ex_dict.h"

// INTERNAL FUNCTION PROTOTYPES

bool searchString(EX_DICT*,unsigned char*,USINT*,USINT);
unsigned char *searchCode(EX_DICT*,USINT,USINT*);

USINT encode(EX_DICT*,unsigned char*,USINT);
unsigned char *decode(EX_DICT*,USINT,USINT*);

// EXTERNAL FUNCTION DEFINITIONS

void EX_LZW_COMPRESS(FILE *in_fp,FILE *out_fp) {
	EX_DICT *dict; // my dictionary built of two way linked lists
	
	unsigned char	*STRING;
	unsigned char	*STRCHAR;
	unsigned char	CHAR;
	USINT	code;
	USINT	stringLen;

	dict = EX_DICT_INIT(); // initialize the dictionary and build first page with roots
	// STRING    = (unsigned char *) malloc (STRING_LEN+1 * sizeof(unsigned char)); // extra for null byte
	// STRCHAR   = (unsigned char *) malloc (STRING_LEN+1 * sizeof(unsigned char)); // extra for null byte
		
	STRING    = NULL;
	stringLen = 0;

	while(fread(&CHAR,sizeof(unsigned char),1,in_fp)) { // read current

		STRCHAR = (unsigned char *) malloc (stringLen+1);

		memcpy(STRCHAR,STRING,stringLen);
		memcpy((STRCHAR+stringLen),&CHAR,sizeof(unsigned char));

		if (searchString(dict,STRCHAR,&code,stringLen+1)) {
			free(STRING);
			STRING = (unsigned char *) malloc (stringLen+1);
			memcpy(STRING,STRCHAR,stringLen+1);
			stringLen++;
		} else {
			if (stringLen > 0) {
				code = encode(dict,STRING,stringLen);
				fwrite(&code,sizeof(USINT),1,out_fp);
			}

			EX_DICT_ADD_PAGE(dict,STRCHAR,stringLen+1);

			free(STRING);
			STRING = (unsigned char *) malloc (sizeof(unsigned char));
			memcpy(STRING,&CHAR,sizeof(unsigned char));
			stringLen = 1;
		}
		free(STRCHAR);
	}

	code = encode(dict,STRING,stringLen);
	fwrite(&code,sizeof(USINT),1,out_fp);

	free(STRING);
	EX_DICT_DELETE(dict);
}

void EX_LZW_DECOMPRESS(FILE *in_fp,FILE *out_fp) {
	EX_DICT *dict; // my dictionary built of two way linked lists
	
	USINT	NCODE;
	USINT	OCODE;
	unsigned char	CODECHAR;
	unsigned char	*OPAT   = NULL;
	unsigned char	*NPAT   = NULL;
	unsigned char	*STRING = NULL;
	USINT 	length;

	dict = EX_DICT_INIT(); // initialize the dictionary and build first page with roots

	fread(&OCODE,sizeof(USINT),1,in_fp); // read K
	// printf("\nocode: %i\n",OCODE);

	STRING = decode(dict,OCODE,&length);
	// printf("\nfirst length: %i\n", length);
	fwrite(STRING,length,1,out_fp); // output pattern

	while(fread(&NCODE,sizeof(USINT),1,in_fp)) {
		//printf("ncode: %i\n",NCODE);

		if (searchCode(dict,NCODE,&length)) {
			NPAT = decode(dict,NCODE,&length);
			// printf("NPAT length: %i\n", length);
			fwrite(NPAT,length,1,out_fp);

			OPAT     = decode(dict,OCODE,&length);
			CODECHAR = NPAT[0];
			STRING   = (unsigned char *) malloc (length+1); // for CODECHAR
			
			memcpy(STRING,OPAT,length);
			memcpy((STRING+length),&CODECHAR,sizeof(unsigned char));

			EX_DICT_ADD_PAGE(dict, STRING, length+1);
			free(STRING);
		} else {
			OPAT     = decode(dict,OCODE,&length);
			CODECHAR = OPAT[0];
			STRING   = (unsigned char *) malloc (length+1); // STRCHR and NULL

			memcpy(STRING,OPAT,length);
			memcpy((STRING+length),&CODECHAR,sizeof(unsigned char));
			
			// printf("STRING length: %i\n", length+1);
			fwrite(STRING,length+1,1,out_fp);
			EX_DICT_ADD_PAGE(dict, STRING, length+1);
			free(STRING);
		}
		OCODE = NCODE;	
	}
	EX_DICT_DELETE(dict);
}

// INTERNAL FUNCTION DEFINITIONS

bool searchString(EX_DICT *dict,unsigned char *query,USINT *code, USINT length) {
	EX_DICT_PAGE *page;
	bool found = false;

	page = dict->head;

	while (!found && page) {
		if (!memcmp(query,page->string,length) && page->length == length) {
			found = true;
			*code = page->code;
			break;
		} else
			page = page->next;
	}

	if (page)
		return true;
	else
		return false;
}

unsigned char *searchCode(EX_DICT *dict,USINT code,USINT *length) {
	EX_DICT_PAGE *page;
	bool found = false;

	page = dict->head;

	while (!found && page) {
		if (code == page->code) {
			found   = true;
			*length = page->length;
			break;
		} else
			page = page->next;
	}

	if (page)
		return page->string;
	else
		return NULL;
}

USINT encode(EX_DICT *dict,unsigned char *string,USINT length) {
	USINT result;

	searchString(dict,string,&result,length);

	return result;
}

unsigned char *decode(EX_DICT *dict,USINT code,USINT *length) {
	unsigned char *result;

	result = searchCode(dict,code,length);

	assert(result);

	return result;
}