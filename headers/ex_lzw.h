#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#ifndef EX_LZW_H
#define EX_LZW_H

void EX_LZW_COMPRESS(FILE *, FILE *);
void EX_LZW_DECOMPRESS(FILE *, FILE *);

#endif