#include <stdio.h> // only for print
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#ifndef EX_BINTREE_H
#define EX_BINTREE_H

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//////////////////                                //////////////////
////////////////// EXTERNAL FUNCTION DESCRIPTIONS //////////////////
//////////////////                                //////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////
// BINARY TREE FUNCTIONS //
///////////////////////////
//
// EX_BINTREE_INIT () : return EX_BINTREE *
// 		Creates a binary tree for with symbols and weight and
//		returns a pointer to the tree.
//
// EX_BINTREE_BUILD (EX_BINTREE *tree, EX_ORDLIST *list, EX_ORDLIST *synlist[]) : return VOID
// 		Builds the binary tree from a one-way linked list of
//		type EX_ORDLIST and 
//
// EX_BINTREE_PRINT (EX_BINTREE *tree, EX_ELEM **elemtable) : return VOID
// 		Prints the tree.
//
// EX_BINTREE_DELETE (EX_BINTREE *tree) : return VOID
// 		Deletes the tree.
//
//
//
////////////////////////////
// ORDERED LIST FUNCTIONS //
////////////////////////////
//
// EX_ORDLIST_INIT () : return EX_ORDLIST
// 		Creates an ordered one way list (ascending) that acts as a
//		sorted stack.
//
// EX_ORDLIST_POP () : return EX_ELEM
// 		Pops the EX_ELEM off the EX_ORDLIST where the
//		EX_ELEM->pair->frequency is lowest.
//
// EX_ORDLIST_PUSH (EX_ORDLIST *list, EX_ELEM *pair) : return VOID
// 		Places pair in list.
//
// EX_ORDLIST_PRINT (EX_ORDLIST *list) : return VOID
// 		Prints list.
//
// EX_ORDLIST_DELETE (EX_ORDLIST *list) : return VOID
// 		Deletes list.
//
//
//
///////////////////////////
// NOSORT LIST FUNCTIONS //
///////////////////////////
//
// EX_NOSORT_INIT () : return EX_NOSORT
// 		Creates a one way list that acts as a stack.
//
// EX_NOSORT_POP () : return EX_ELEM
// 		Pops the bit off the stack.
//
// EX_NOSORT_PUSH (EX_NOSORT *list, bool bit) : return VOID
// 		Places bit on stack.
//
// EX_NOSORT_PRINT (EX_NOSORT *list) : return VOID
// 		Prints list.
//
// EX_NOSORT_DELETE (EX_NOSORT *list) : return VOID
// 		Deletes list.
//
//
//
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////                   //////////////////
////////////////// STRUCTURE DEFINES //////////////////
//////////////////                   //////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

/////////////
// EX_ELEM //
/////////////

typedef struct _ex_elem
{
	struct _ex_elem					*left,*right,*parent,*next;
	unsigned char					symbol;
	unsigned int					frequency;
	bool							isNode;
} EX_ELEM;

////////////////
// EX_BINTREE //
////////////////

typedef struct _ex_bintree
{
	EX_ELEM							*head;
} EX_BINTREE;

////////////////
// EX_ORDLIST //
////////////////

typedef struct _ex_ordlist
{
	EX_ELEM							*head;
} EX_ORDLIST;

///////////////
// EX_NOSORT //
///////////////

typedef struct _ex_ordlist_nosort_elem
{
	struct _ex_ordlist_nosort_elem	*parent;
	bool							bit;			
} EX_NOSORT_ELEM;

typedef struct _ex_ordlist_nosort
{
	EX_NOSORT_ELEM			*head;
} EX_NOSORT;

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//////////////////                     //////////////////
////////////////// FUNCTION PROTOTYPES //////////////////
//////////////////                     //////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

EX_BINTREE *		EX_BINTREE_INIT();
void				EX_BINTREE_BUILD(EX_BINTREE *, EX_ORDLIST *, EX_ELEM **);
void				EX_BINTREE_PRINT(EX_BINTREE *, EX_ELEM **);
void				EX_BINTREE_DELETE(EX_BINTREE *);

EX_ORDLIST *		EX_ORDLIST_INIT();
EX_ELEM *			EX_ORDLIST_POP(EX_ORDLIST *);
void				EX_ORDLIST_PUSH(EX_ORDLIST *, EX_ELEM *);
void				EX_ORDLIST_PRINT(EX_ORDLIST *);
void				EX_ORDLIST_DELETE(EX_ORDLIST *);

EX_NOSORT *			EX_NOSORT_INIT();
bool				EX_NOSORT_POP(EX_NOSORT *);
void				EX_NOSORT_PUSH(EX_NOSORT *, bool);
void				EX_NOSORT_PRINT(EX_NOSORT *);
void				EX_NOSORT_DELETE(EX_NOSORT *);

#endif
