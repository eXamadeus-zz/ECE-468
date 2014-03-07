#include <stdio.h>
#include <stdbool.h>

#ifndef EX_RLE_H
#define EX_RLE_H

void EX_RLE_COMPRESS(FILE *, FILE *);
void EX_RLE_DECOMPRESS(FILE *, FILE *);

#endif