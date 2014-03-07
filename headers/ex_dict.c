#include "ex_dict.h"

// INTERNAL FUNCTION PROTOTYPES

void __FREE_DICT_PAGE (EX_DICT_PAGE *);
void __FREE_DICT (EX_DICT *);

// EXTERNAL FUNCTION DEFINITIONS

EX_DICT * EX_DICT_INIT() {
	EX_DICT			*dict;
	USINT			count;
	unsigned char	*string;

	dict = (EX_DICT *) malloc (sizeof(EX_DICT));
	string = (unsigned char *) malloc (sizeof(USINT));

	dict->head     = NULL;
	dict->tail     = NULL;
	dict->currCode = 0;

	for (count=0; count<256; count++)
	{
		EX_DICT_ADD_PAGE(dict, string, 1);
		string[0] = count+1;
	}

	free(string);

	return dict;
}

void EX_DICT_ADD_PAGE(EX_DICT *dict, unsigned char *string, USINT length) {
	if (!dict || !string) exit(3);
	
	EX_DICT_PAGE	*page;
	unsigned char			*stored;

	page   = (EX_DICT_PAGE *) malloc (sizeof(EX_DICT_PAGE));
	stored = (unsigned char *) malloc (length*sizeof(unsigned char));

	memcpy(stored, string, length);

	page->string = stored;
	page->code   = (dict->currCode)++;
	page->length = length;

	if (dict->head == NULL) {
		dict->head = page;
		dict->tail = page;
	} else {
		page->prev = dict->tail;
		dict->tail = page;
		page->next = NULL;
		page->prev->next = page;
	}
}

void EX_DICT_PRINT(EX_DICT *dict) {
	if (!dict) return;
	
	EX_DICT_PAGE *ptr = dict->head;

	printf("\n");

	while (ptr->code <= 0xFF) { // print all pages
		ptr=ptr->next;
	}

	while (ptr) { // print all pages
		printf("code: 0x%X\t(%i)\tlen: %i\t(%s)\n",
			ptr->code, ptr->code, ptr->length, ptr->string);
		ptr = ptr->next;
	}

}

void EX_DICT_DELETE(EX_DICT *dict) {
	// EX_DICT_PRINT(dict);
	__FREE_DICT(dict);
}

void __FREE_DICT (EX_DICT *dict) {
	if (!dict) return;
	
	EX_DICT_PAGE *ptr;

	while (dict->head != NULL) { // free all pages
		ptr = dict->head;
		dict->head = ptr->next;
		free(ptr->string);
		free(ptr);
	}

	free(dict); // free the dictionary
}