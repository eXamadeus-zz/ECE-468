#include "ex_huffenc.h"
#include "ex_bintree.h"

#include <stdio.h>

void shiftNwrite(FILE *, char *);

void huffencode ( FILE * source, FILE * encoded){

    EX_ORDLIST *	hufflist;
    EX_NOSORT * 	binarystack;
    EX_BINTREE *	hufftree;
    EX_ELEM *		tracer, * nodelist[256] = {0};
    EX_ELEM *		pair;
    int			symcount = 0, codecount = 0, stats[256] = {0}, 
                            remaining = 0, stacksize = 0, i;
    unsigned char	current, output = 0;
    // long int 		filesize;
    bool		pathbit;

    // file preparations
    assert(source);
    assert(encoded);

    // generate symbol statistics from file
    while(fscanf(source, "%c", &current) != EOF){
        (stats[current])++;
        codecount++;
    } 

    // build tree
    hufflist = EX_ORDLIST_INIT();
    for(i=0 ; i<256 ; i++){
        if(stats[i]){
            pair = (EX_ELEM *) calloc(1,sizeof(EX_ELEM));
            pair->symbol    = i;
            pair->frequency = stats[i];
            EX_ORDLIST_PUSH (hufflist, pair);
            symcount++;
        }
    }
    hufftree = EX_BINTREE_INIT();

// EX_ORDLIST_PRINT(hufflist);

    EX_BINTREE_BUILD (hufftree, hufflist, nodelist);
    binarystack = EX_NOSORT_INIT();

    // write header to encode file
// printf("symcount %i\tcodecount %i\n", symcount, codecount);
    fwrite(&symcount, sizeof(int), 1, encoded);
    fwrite(&codecount, sizeof(int), 1, encoded);
    for(i=0 ; i<256 ; i++){
        if(stats[i]){
            fwrite(&i, sizeof(char), 1, encoded);
            fwrite(&stats[i], sizeof(int), 1, encoded);
            // shiftNwrite(encoded, (char *)&stats[i]);
            // fprintf(encoded, "%c%i", i, stats[i]);
        }
    }

    // compress
    fseek(source, 0L, SEEK_SET);
    remaining = 7;
    while(fscanf(source, "%c", &current) != EOF){
        tracer = nodelist[current];
        assert(tracer->symbol == current);
// printf("Leaf to trunk for\t\t");
// if (current == 0x0A)
    // printf("(\\n)\t");
// else
    // printf("(%c)\t", current);
// printf("0x%X\tbin: ", current);
        while(tracer->parent != NULL){
            if(tracer->parent->left == tracer){
// printf("0");
                pathbit = 0;
                EX_NOSORT_PUSH(binarystack, pathbit);
                stacksize++;
            }
            else{
// printf("1");
                pathbit = 1;
                EX_NOSORT_PUSH(binarystack, pathbit);
                stacksize++;
            }
            tracer = tracer->parent;
        }
// printf("\n");
        while(stacksize){
            stacksize--;
            output += (int)EX_NOSORT_POP(binarystack) * 128;

            // output buffer
            if(remaining == 0){
                fwrite(&output, sizeof(char), 1, encoded);
                output = 0;
                remaining = 7;
            }
            else{
                output = output >> 1;
                remaining--;
            }
        }
    }
    if(remaining != 7){
        output = output >> remaining;
        fwrite(&output, sizeof(char), 1, encoded);
    }

    //cleanup
    // filesize = ftell(encoded);
    // printf("\nBytes in file %li\n", filesize);
    EX_NOSORT_DELETE(binarystack); 
    EX_BINTREE_DELETE(hufftree);
    EX_ORDLIST_DELETE(hufflist);

    return;
}

void shiftNwrite(FILE *out, char *num)
{
    fwrite(&num[0],1,1,out);
    fwrite(&num[1],1,1,out);
    fwrite(&num[2],1,1,out);
    fwrite(&num[3],1,1,out);
}
