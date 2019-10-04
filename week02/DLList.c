// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2014, August 2015

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "DLList.h"

// data structures representing DLList

typedef struct DLListNode {
	char *value;        /**< value of this list item (string) */
	struct DLListNode *prev;
	                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode;

typedef struct DLListRep {
	size_t nitems;      /**< count of items in list */
	DLListNode *first;  /**< first node in list */
	DLListNode *curr;   /**< current node in list */
	DLListNode *last;   /**< last node in list */
} DLListRep;

static DLListNode *newDLListNode(char *it);
static void freeDLListNode(DLListNode *node);

/** Create a new, empty DLList. */
DLList newDLList (void)
{
	DLListRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList");
	new->curr = new->first = new->last = NULL;
	new->nitems = 0;
	return new;
}

/** Release all resources associated with a DLList. */
void freeDLList (DLList L)
{
	if (L == NULL) return;

	DLListNode *curr = L->first;
	while (curr != NULL) {
		DLListNode *next = curr->next;
		freeDLListNode (curr);
		curr = next;
	}
	free (L);
}

/** Create a new DLListNode (private function) */
static DLListNode *newDLListNode (char *it)
{
	DLListNode *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList node");
	new->value = strdup (it);
	new->prev = new->next = NULL;
	return new;
}

/** Release a DLListNode (private function) */
static void freeDLListNode (DLListNode *node)
{
	if (node == NULL) return;
	free (node->value);
	free (node);
}

/** Trim off newline from strings (private function)
 * This is needed for getDLList() because fgets(3). */
static char *trim (char *s)
{
	char *nl = strrchr (s, '\n');
	if (nl != NULL) *nl = '\0';
	return s;
}

/** Create an DLList by reading items from a file.
 * Assume that the file is open for reading. */
DLList getDLList (FILE *in)
{
	DLList L = newDLList ();
	char line[BUFSIZ];
	while (fgets (line, BUFSIZ, in) != NULL) {
		DLListNode *new = newDLListNode (trim (line));
		if (L->last == NULL) {
			L->first = L->last = new;
		} else {
			L->last->next = new;
			new->prev = L->last;
			L->last = new;
		}
		L->nitems++;
	}
	L->curr = L->first;
	return L;
}

/** Display a DLList to file, one item per line.
 * Assume that the file is open for writing. */
void putDLList (FILE *out, DLList L)
{
	assert (out != NULL);
	assert (L != NULL);
	for (DLListNode *curr = L->first; curr != NULL; curr = curr->next)
		fprintf (out, "%s\n", curr->value);
}

/** Check internal consistency of a DLList. */
bool validDLList (DLList L)
{
	if (L == NULL) {
		fprintf (stderr, "DLList is null\n");
		return false;
	}

	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL || L->curr != NULL) {
			fprintf (stderr, "Non-null pointers in empty list\n");
			return false;
		}
	} else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL || L->curr == NULL) {
			fprintf (stderr, "Null pointers in non-empty list\n");
			return false;
		}
	}

	// check scanning forward through list
	size_t count = 0;
	for (DLListNode *curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf (
				stderr,
				"Invalid forward link into node (%s)\n",
				curr->value);
			return 0;
		}

		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf (
				stderr,
				"Invalid backward link into node (%s)\n",
				curr->value);
			return 0;
		}
		count++;
	}

	if (count != L->nitems) {
		fprintf (
			stderr,
			"Forward count mismatch; counted=%zu, nitems=%zu\n",
			count,
			L->nitems);
		return false;
	}

	// check scanning backward through list
	count = 0;
	for (DLListNode *curr = L->last; curr != NULL; curr = curr->prev)
		count++;

	if (count != L->nitems) {
		fprintf (
			stderr,
			"Backward count mismatch; counted=%zu, nitems=%zu\n",
			count,
			L->nitems);
		return false;
	}
	// nothing went wrong => must be ok
	return true;
}

/** Return the item at current position. */
char *DLListCurrent (DLList L)
{
	assert (L != NULL);
	return L->curr != NULL ? L->curr->value : NULL;
}

/** Move current position (+ve forward, -ve backward).
 * Return false if we reach the end of list during the move. */
bool DLListMove (DLList L, int n)
{
	assert (L != NULL);
	if (L->curr == NULL) {
		return true;
	} else if (n > 0) {
		while (n > 0 && L->curr->next != NULL) {
			L->curr = L->curr->next;
			n--;
		}
	} else if (n < 0) {
		while (n < 0 && L->curr->prev != NULL) {
			L->curr = L->curr->prev;
			n++;
		}
	}

	return L->curr != L->first && L->curr != L->last;
}

/** move to specified position in list
 * i'th node, assuming first node has i==1 */
bool DLListMoveTo (DLList L, int i)
{
	assert (L != NULL);
	assert (i > 0);
	L->curr = L->first;
	return DLListMove (L, i - 1);
}

/** insert an item before current item
 * new item becomes current item */
void DLListBefore (DLList L, char *it)
{
	assert (L != NULL);
	DLListNode *new = newDLListNode(it);
	L->nitems++;
	if (L->curr == NULL)
	{
		
		L->curr = new;
		L->first = L->last = new;
		return;
	}
	// If the item should be the first new item in the list. 
	if (L->curr == L->first)
	{
		
		new->next = L->curr;
		L->curr->prev = new;
		L->curr = new;
		L->first = new;
		return;
	}
	
	new->next = L->curr;
	new->prev = L->curr->prev;
	L->curr->prev->next = new;
	L->curr->prev = new;
	L->curr = new;

}

/** insert an item after current item
 * new item becomes current item */
void DLListAfter (DLList L, char *it)
{
	assert (L != NULL);
	DLListNode *new = newDLListNode(it);
	L->nitems++;
	// Checking if it is a null list 
	if (L->curr == NULL)
	{
		L->curr = new;
		L->first = L->last = new;
		return;
	}
	// adding after the last node 
	if (L->curr == L->last)
	{
		new->prev = L->curr;
		L->curr->next = new;
		L->curr = new;
		L->last = new;
		return;
	}

	new->prev = L->curr;
	L->curr->next->prev = new;
	new->next = L->curr->next;
	L->curr->next = new;
	L->curr = new;
}

/** delete current item
 * new item becomes item following current
 * if current was last, current becomes new last
 * if current was only item, current becomes null */
void DLListDelete (DLList L)
{
	assert (L != NULL);
	// Null case
	if (L->curr == NULL) 
	{ 	
		return;
	}

	// if there is only one item in the list 
	if (L->nitems == 1)
	{
		DLListNode *temp = L->curr;
		free (temp->value);
		free(temp);
		L->curr = L->first = L->last = NULL;
		L->nitems--;
		return;
	}
	// call to the last node 
	if (L->curr == L->last)
	{
		DLListNode *temp = L->curr;
		L->last = temp->prev;
		L->last->next = NULL;
		temp->prev = NULL;
		L->curr = L->last;
		free (temp->value);
		free(temp);
		L->nitems--;
		return;
	}
	// deleting the first node 
	if (L->curr == L->first)
	{
		DLListNode *temp = L->curr;
		L->first = temp->next;
		L->first->prev = NULL;
		L->last->next = NULL;
		temp->next = NULL;
		L->curr = L->first;
		free (temp->value);
		free(temp);
		L->nitems--;
		return;
	}
	// Deleting a node in between the list 
	DLListNode *temp = L->curr;
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	L->curr = temp->next;
	temp->next = temp->prev = NULL;
	free (temp->value);
	free(temp);
	L->nitems--;

}

/** return number of elements in a list */
size_t DLListLength (DLList L)
{
	return L->nitems;
}

/** is the list empty? */
bool DLListIsEmpty (DLList L)
{
	return L->nitems == 0;
}
