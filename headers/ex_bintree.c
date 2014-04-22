#include "ex_bintree.h"

// Private
static void	__EX_ELEM_DELETE (EX_ELEM *);

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////                    //////////////////
////////////////// BINTREE PROTOTYPES //////////////////
//////////////////                    //////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// EX_BINTREE_INIT () : return EX_BINTREE *
// 		Creates a binary tree for with symbols and weight and
//		returns a pointer to the tree.
EX_BINTREE *
EX_BINTREE_INIT()
{
	EX_BINTREE *tree;

	tree       = (EX_BINTREE *) malloc (sizeof(EX_BINTREE));
	tree->head = NULL;
	
	return tree;
}

// EX_BINTREE_BUILD (EX_BINTREE *tree, EX_ORDLIST *list, EX_ORDLIST *elemtable[]) : return VOID
// 		Builds the binary tree from a one-way linked list of
//		type EX_ORDLIST and 
void
EX_BINTREE_BUILD(EX_BINTREE *tree, EX_ORDLIST *list, EX_ELEM *elemtable[])
{
	EX_ELEM	*newElem, *left, *right;

	left = right = NULL;

	newElem = list->head;

	do
	{
		newElem->isNode = false;
		newElem = newElem->next;
	}  while (newElem);

	do
	{
		left  = EX_ORDLIST_POP(list);
		right = EX_ORDLIST_POP(list);
		
		if (left && !left->isNode)
		{
			elemtable[(int)left->symbol] = left;
			left->left = NULL;
			left->right = NULL;
		}
		if (right && !right->isNode)
		{
			elemtable[(int)right->symbol] = right;
			right->left = NULL;
			right->right = NULL;
		}

		if (!right)
		{ // final tree
			tree->head = left;
			break;
		}
		else
		{
			newElem            = (EX_ELEM *) malloc (sizeof(EX_ELEM));
			newElem->left      = left;
			newElem->right     = right;
			newElem->isNode    = true;
			newElem->parent    = NULL;

			newElem->frequency = left->frequency + right->frequency;

			left->parent  = newElem;
			right->parent = newElem;

			EX_ORDLIST_PUSH(list, newElem);
		}
	} while (left != NULL);

	// EX_BINTREE_PRINT(tree,elemtable);
}

// EX_BINTREE_PRINT (EX_BINTREE *tree, EX_ELEM **elemtable) : return VOID
// 		Prints the tree.  OK, it doesn't actually work.
void
EX_BINTREE_PRINT(EX_BINTREE *tree, EX_ELEM *elemtable[])
{
	printf("\n(This doesn't work...) ELEMLIST:\n");

	int i;
	for (i=0; i<256; i++)
	{
		if (elemtable[i])
		{
			printf("SYML: ADD  freq: %d\tsymbol: %X\t(%d)\t",
				elemtable[i]->frequency,
				elemtable[i]->symbol,
				elemtable[i]->symbol);
			if (elemtable[i]->symbol == 0x0A)
				printf("(\\n)");
			else
				printf("(%c)", elemtable[i]->symbol);
			printf(elemtable[i]->isNode ?
				"\n" :
				"\tLEAF\n");
		}
	}
}

// EX_BINTREE_DELETE (EX_BINTREE *tree) : return VOID
// 		Deletes the tree.
void
EX_BINTREE_DELETE(EX_BINTREE *tree)
{
	if (!tree) return; // no tree

	__EX_ELEM_DELETE(tree->head);

	free(tree);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////                    //////////////////
////////////////// ORDLIST PROTOTYPES //////////////////
//////////////////                    //////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// EX_ORDLIST_INIT () : return EX_ORDLIST
// 		Creates an ordered one way list (ascending) that acts as a
//		sorted stack.
EX_ORDLIST *
EX_ORDLIST_INIT ()
{
	EX_ORDLIST *list;

	list = (EX_ORDLIST *) malloc (sizeof(EX_ORDLIST));

	list->head = NULL;

	return list;
}

// EX_ORDLIST_POP () : return EX_ELEM
// 		Pops the EX_ELEM off the EX_ORDLIST where the
//		EX_ELEM->pair->frequency is lowest.
EX_ELEM *
EX_ORDLIST_POP(EX_ORDLIST *list)
{
	if (!list || !list->head)
		return NULL;

	EX_ELEM	*old;

	old        = list->head;
	list->head = old->next;

	// if (old) printf("list: POP  freq: %d\tsymbol: %X\n", old->frequency, old->symbol);

	return old;
}

// EX_ORDLIST_PUSH (EX_ORDLIST *list, EX_ELEM *pair) : return VOID
// 		Places pair in list.
void
EX_ORDLIST_PUSH (EX_ORDLIST *list, EX_ELEM *elem)
{
	EX_ELEM *curr, *prev;

	assert(list);
	assert(elem);

	// printf("list: PUSH freq: %d\tsymbol: %X\n",
	// 		elem->frequency, elem->symbol);

	if (list->head == NULL)
	{
		list->head       = elem;
		list->head->next = NULL;
		return;
	}

	curr = list->head;
	prev = NULL;

	while (elem->frequency > curr->frequency)
	{
		if (curr->next)
		{
			prev = curr;
			curr = curr->next;
		}
		else
		{
			prev = curr;
			curr = NULL;
			break;
		}
	}
	
	EX_ELEM *nodeStop = elem;

	while (nodeStop->left)
		nodeStop = nodeStop->left;

	while (curr && (elem->frequency == curr->frequency) &&
		(nodeStop->symbol > curr->symbol))
	{
		if (curr->next)
		{
			prev = curr;
			curr = curr->next;
		}
		else
		{
			prev = curr;
			curr = NULL;
			break;
		}
	}

	elem->next = curr;

	if (prev)
		prev->next = elem;
	else
		list->head = elem;
}

// EX_ORDLIST_PRINT (EX_ORDLIST *list) : return VOID
// 		Prints list.
void
EX_ORDLIST_PRINT(EX_ORDLIST *list)
{
	assert(list);

	EX_ELEM *ptr = list->head;

	while (ptr != NULL)
	{
		printf("PUSH freq: %d\t\t", ptr->frequency);
		if (ptr->symbol == 0x0A)
				printf("(\\n)\t");
		else
			printf("(%c)\t", ptr->symbol);
		printf("0x%X\n", ptr->symbol);
		ptr = ptr->next;
	}

	printf("\n");
}

// EX_ORDLIST_DELETE (EX_ORDLIST *list) : return VOID
// 		Deletes list.
void
EX_ORDLIST_DELETE(EX_ORDLIST *list)
{
	if (!list || !list->head) return;

	EX_ELEM *old;

	while (list->head != NULL)
	{
		old        = list->head;
		list->head = old->next;
		if (old) printf("list: DEL  freq: %d\tsymbol: %X\n",
			old->frequency, old->symbol);
		free(old);
	}

	free(list);
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////                   //////////////////
////////////////// NOSORT PROTOTYPES //////////////////
//////////////////                   //////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

// EX_NOSORT_INIT () : return EX_NOSORT
// 		Creates a one way list that acts as a stack.
EX_NOSORT *
EX_NOSORT_INIT ()
{
	EX_NOSORT *list;

	list = (EX_NOSORT *) malloc (sizeof(EX_NOSORT));

	list->head = NULL;

	return list;
}

// EX_NOSORT_POP () : return EX_ELEM
// 		Pops the bit off the stack.
bool
EX_NOSORT_POP(EX_NOSORT *list)
{
	if (!list || !list->head)
		return NULL;

	EX_NOSORT_ELEM	*old;
	bool			bit;

	old        = list->head;
	bit        = old->bit;
	list->head = old->parent;

	free(old);

	return bit;
}

// EX_NOSORT_PUSH (EX_NOSORT *list, bool bit) : return VOID
// 		Places bit on stack.
void
EX_NOSORT_PUSH (EX_NOSORT *list, bool bit)
{
	assert(list);

	EX_NOSORT_ELEM *newElem/*, *tail*/;

	// tail = list->head;

	// while(tail && tail->parent)
	// 	tail = tail->parent;

	// newElem         = (EX_NOSORT_ELEM *) malloc (sizeof(EX_NOSORT_ELEM));
	// newElem->bit    = bit;
	// newElem->parent = NULL;
	
	// if (tail)
	// 	tail->parent = newElem;
	// else
	// 	list->head = newElem;

	newElem         = (EX_NOSORT_ELEM *) malloc (sizeof(EX_NOSORT_ELEM));
	newElem->bit    = bit;
	newElem->parent = list->head;
	list->head      = newElem;
}

// EX_NOSORT_PRINT (EX_NOSORT *list) : return VOID
// 		Prints list.
void
EX_NOSORT_PRINT(EX_NOSORT *list)
{
	assert(list);

	EX_NOSORT_ELEM *ptr = list->head;

	while (ptr != NULL)
	{
		printf("address: 0x%p bit: %d\n", ptr, (int)ptr->bit);
		ptr = ptr->parent;
	}
}

// EX_NOSORT_DELETE (EX_NOSORT *list) : return VOID
// 		Deletes list.
void
EX_NOSORT_DELETE(EX_NOSORT *list)
{
	if (!list->head) return;

	EX_NOSORT_ELEM *ptr;

	while (list->head != NULL)
	{
		ptr  = list->head;
		list->head = ptr->parent;
		free(ptr);
	}

	free(list);
}

/////////////////////// INTERNAL USE ONLY //////////////////////////
//   __EX_BINTREE_DELETE_ELEM (EX_ELEM *elem) : return VOID       //
//      Delete the tree elements and all child elements.          //
////////////////////////////////////////////////////////////////////
static void
__EX_ELEM_DELETE(EX_ELEM *elem)
{
	if (!elem) return;

	__EX_ELEM_DELETE(elem->left);
	__EX_ELEM_DELETE(elem->right);

	if (elem) free(elem);
}