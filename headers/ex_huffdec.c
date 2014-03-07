#include "ex_huffdec.h"
#include "ex_bintree.h"

void shiftNread(FILE *in, char *num);

void huffdecode ( FILE * encoded , FILE * decoded ){

    EX_ORDLIST * 	hufflist;
    EX_BINTREE *	hufftree;
    EX_ELEM *		tracer, * nodelist[256] = {0};
    EX_ELEM *		pair;
    int			symcount, codecount, remaining = 0, i;
    unsigned char	mask = 1, current;
    // long int		filesize;

    // file preparations
    assert(encoded);
    assert(decoded);

    fread(&symcount, sizeof(int), 1, encoded);
    fread(&codecount, sizeof(int), 1, encoded);
    assert(feof(encoded) == 0);

    // build tree
    hufflist = EX_ORDLIST_INIT();
// printf("symcount %i\tcodecount %i\n", symcount, codecount);
    for( i=0 ; i<symcount; i++){
        assert(pair = (EX_ELEM *)malloc(sizeof(EX_ELEM)));
        fread(&(pair->symbol), sizeof(char), 1, encoded);
        fread(&(pair->frequency), sizeof(int), 1, encoded);
        pair->left = pair->right = pair->parent = pair->next = NULL;
        pair->isNode = false;
        assert(feof(encoded) == 0);
        EX_ORDLIST_PUSH (hufflist, pair);
    }

    hufftree = EX_BINTREE_INIT();

    // printf("\n");
    // EX_ORDLIST_PRINT(hufflist);

    EX_BINTREE_BUILD (hufftree, hufflist, nodelist);

    // decompress
    fread(&current, sizeof(char), 1, encoded);
    remaining = 7;
    for( i=0 ; i<codecount; i++){
        tracer = hufftree->head;
        // trace tree until you reach terminal node
        while( tracer->left != NULL && tracer->right != NULL ){

            if((current & mask) == 0) {
                tracer = tracer->left;
// printf("0");
            }
            else
            {
                tracer = tracer->right;
// printf("1");
            }

            //get more encoded data as necessary
            if(remaining == 0){            
                fread(&current, sizeof(char), 1, encoded);
                // assert(feof(encoded) == 0);
                remaining = 7;
            }
            else{
                current = current >> 1;
                remaining--;;
            }
        }
        fwrite(&(tracer->symbol), sizeof(char), 1, decoded);
// if (tracer->symbol == 0x0A)
// printf("\tsym: %X (\\n) \n", tracer->symbol);
// else
// printf("\tsym: %X (%c) \n", tracer->symbol, tracer->symbol);
    }

    // cleanup
    // filesize = ftell(decoded);
    // printf("Bytes in file %li\n", filesize);
    EX_BINTREE_DELETE(hufftree);
    EX_ORDLIST_DELETE(hufflist);

    return;
}

void shiftNread(FILE *in, char *num)
{
    fread(&num,1,4,in);
}

