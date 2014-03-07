#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef EX_DICT_H
#define EX_DICT_H

#define STRING_LEN 1000
typedef unsigned short int USINT;

// TYPE DEFINES

typedef struct _ex_dict {
	struct _ex_dict_page 	*head,*tail;
	USINT					currCode;
} EX_DICT;

typedef struct _ex_dict_page {
	struct _ex_dict_page	*prev,*next;
	unsigned char			*string;
	USINT					code;
	USINT					length;
} EX_DICT_PAGE;

// FUNCTION PROTOTYPES

EX_DICT * EX_DICT_INIT();
void EX_DICT_ADD_PAGE(EX_DICT *, unsigned char *, USINT);
void EX_DICT_DELETE(EX_DICT *);

void EX_DICT_PRINT(EX_DICT *);

#endif