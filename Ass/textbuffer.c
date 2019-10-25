
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"


typedef struct TBNode {
	char *value;    // Store the string
	struct TBNode *prev; 		// Pointer to previous node
	struct TBNode *next; 		// Pointer to next node
} *Node;

struct textbuffer {
	int nitems;      /**< count of items in list */
	Node first;  		/**< first node in list */
	Node last;   		/**< last node in list */
	int size;			/* Saves the size for the text string */
};

// Static function prototypes
static void freeNode(Node curr);
static char *showLineNumber(TB tb);
static int checkSize(TB tb);
static void pasteLastTB(TB tb1, TB tb2);
/*
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text) {

	TB buffer = malloc(sizeof(struct textbuffer));
	if (buffer == NULL) {
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	if (text != NULL) {
		buffer->size = strlen(text);
	}else
	{
		buffer->size = 0;
	}

	buffer->nitems = 0;
	buffer->first = buffer->last = NULL;

	if (text == NULL) {
		return buffer;
	}
	char *textTemp = malloc(strlen(text)*sizeof(char) + 1);
	if (textTemp == NULL){
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	strcpy(textTemp, text);

	char *string = strtok(textTemp, "\n");

	while (string != NULL) {
		// printf("%s \n", string);
		Node new = malloc(sizeof(struct TBNode)*sizeof(char));
		if (new == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		new->value = malloc(strlen(string)*sizeof(char) + 1);
		new->next = new->prev = NULL;
		if (new->value == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		strcpy(new->value, string);
		if (buffer->last == NULL) {
			buffer->first = buffer->last = new;
		}else
		{
			buffer->last->next = new;
			new->prev = buffer->last;
			buffer->last = new;
		}
		string = strtok(NULL, "\n");
		buffer->nitems++;
		// printf("%d : %s\n", buffer->nitems, buffer->last->value);
	}
	free(textTemp);
	return buffer;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB(TB tb) {
	if (tb == NULL) {
		return;
	}
	Node curr = tb->first;
	while (curr != NULL) {
		Node next = curr->next;
		freeNode (curr);
		curr = next;
	}
	free (tb);
}

// Private function to free memory used by a node
static void freeNode(Node curr) {
	if (curr == NULL) {
		return;
	}
	free(curr->value);
	free(curr);
}
/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB(TB tb, bool showLineNumbers) {
	if (tb->first == NULL) {
		return NULL;
	}
	if (showLineNumbers == true) {
		return showLineNumber(tb);
	}
	char *string = calloc(1, (tb->size)*sizeof(char) + 3);
	if (string == NULL) {
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	Node temp = tb->first;

	while (temp != NULL) {
		strcat(string, temp->value);
		strcat(string, "\n");
		temp = temp->next;
	}

	return string;
}

// Returns the size for the string
static int checkSize(TB tb) {
	int count = 0;
	int n = tb->nitems;
	while(n != 0) {
        n /= 10;
        ++count;
    }
	n = count + 2;
	count = (tb->size + n);
	return count;
}

// Returns a string with line numbers
static char *showLineNumber(TB tb) {
	char *numString = calloc(1, checkSize(tb)*sizeof(char) + 3*tb->nitems);
	if (numString == NULL) {
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	int count = 0;
	char *num = NULL;
	char text[3];

	Node temp = tb->first;

	while (temp != NULL) {
		count++;
		sprintf(text, "%d", count);
		num = text;
		strcat(numString, num);
		strcat(numString, ". ");
		strcat(numString, temp->value);
		strcat(numString, "\n");
		temp = temp->next;
	}

	return numString;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB(TB tb) {
	return tb->nitems;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB(TB tb, int from, int to, char *prefix) {
	if (from < 1 || to > tb->nitems) {
		fprintf(stderr, "Lines out of range");
		abort();
	}else if (from > to) {
		fprintf(stderr, "Incorrect line range");
		abort();
	}
	int count = 1;
	Node temp = tb->first;
	while (count < from ) {
		count++;
		temp = temp->next;
	}
	
	while (count <= to) {
		count++;
		char *string = calloc(1, (2 + strlen(temp->value) + strlen(prefix)) * sizeof(char));
		strcpy(string, prefix);
		strcat(string, temp->value);
		free(temp->value);
		temp->value = string;
		temp = temp->next;
	}
	tb->size = tb->size + ((2 + strlen(prefix)) * tb->nitems);
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB(TB tb1, int pos, TB tb2) {
	if (pos > tb1->nitems + 1) {
		fprintf(stderr, "Line out of range");
		abort();
	}
	if (tb1->nitems == 0) {
		tb1->first = tb2->first;
		tb1->last = tb2->last;
		tb1->size = tb2->size;
		free(tb2);
		return;
	}
	if (tb2->nitems == 0) {
		free(tb2);
		return;
	}
	int count = 1;
	Node temp = tb1->first;
	while (count < (pos - 1)) {
		count++;
		temp = temp->next;
	} 

	if (temp == tb1->first && pos == 1) {
		tb1->first = tb2->first;
		tb2->last->next = temp;
		temp->prev = tb2->last;
	}else if (temp == tb1->last) {
		temp->next = tb2->first;
		tb2->first->prev = temp;
		tb1->last = tb2->last;
	}else {
		Node new = temp->next;
		temp->next = tb2->first;
		tb2->first->prev = temp;
		tb2->last->next = new;
		new->prev = tb2->last;
	}
	tb1->nitems += tb2->nitems;
	tb1->size += tb2->size;
	if (tb1->last == NULL) {
		tb1->last = tb2->last;
	}
	free(tb2);
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB(TB tb1, int pos, TB tb2) {
	if (pos > tb1->nitems + 1) {
		fprintf(stderr, "Line out of range");
		abort();
	}
	if (tb2->nitems == 0) {
		return;
	}
	if (pos == tb1->nitems + 1)
	{
		pasteLastTB(tb1, tb2);
		return;
	}
	
	int count = 1;
	Node temp = tb1->first;
	Node last = NULL;
	while (count < (pos - 1)) {
		count++;
		temp = temp->next;
	}
	if (pos == 1) {
		last = temp;
	}else {
		last = temp->next;
	}
	int found = 0;
	Node first2 = tb2->first;
	while (first2 != NULL) {	
		if (temp == tb1->first && pos == 1 && found == 0) {
			Node new = malloc(sizeof(struct TBNode));
			new->value = calloc(1, (strlen(first2->value) + 1)*sizeof(char));
			strcpy(new->value, first2->value);
			new->prev = NULL;
			tb1->first = new;
			temp = new;
			first2 = first2->next;
			found++;
		}else {
			Node new = malloc(sizeof(struct TBNode));
			new->value = calloc(1, (strlen(first2->value) + 1)*sizeof(char));
			strcpy(new->value, first2->value);
			new->prev = temp;
			temp->next = new;
			temp = new;
			first2 = first2->next;
		}
		tb1->size += strlen(temp->value);
	}
	temp->next = last;
	last->prev = temp;
	tb1->nitems += tb2->nitems;
	if (tb1->last == NULL) {
		tb1->last = last;
	}
}

static void pasteLastTB(TB tb1, TB tb2) {
	Node last = tb2->first;
	for (int i = 0; i < tb2->nitems; i++)
	{
		Node new = malloc(sizeof(struct TBNode));
		new->value = calloc(1, (strlen(last->value) + 1)*sizeof(char));
		strcpy(new->value, last->value);
		new->next = NULL;
		tb1->last->next = new;
		new->prev = tb1->last;
		last = last->next;
		tb1->last = new;
		tb1->size += strlen(new->value);
		tb1->nitems++;
	}
}
/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB(TB tb, int from, int to) {
	if (to < from)
		return NULL;
	if (from < 1 || to > tb->nitems) {
		fprintf(stderr, "Lines out of range\n");
		abort();
	}
	if (tb->nitems == 0) {
		fprintf(stderr, "Can not delete lines in an empty buffer\n");
		abort();
	}
	TB buffer = malloc(sizeof(struct textbuffer));
	if (buffer == NULL) {
		fprintf(stderr, "Error in allocating memory\n");
		abort();
	}
	buffer->nitems = 0;
	buffer->size = 0;
	buffer->first = buffer->last = NULL;
	int count = 1;
	Node temp = tb->first;
	while (count < from) {
		count++;
		temp = temp->next;
	}
	while(count <= to ) {
		Node new = malloc(sizeof(struct TBNode));
		if (new == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		new->value = calloc(1, strlen(temp->value)*sizeof(char) + 1);
		new->next = new->prev = NULL;
		if (new->value == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		strcpy(new->value, temp->value);
		if (buffer->last == NULL) {
			buffer->first = buffer->last = new;
		}else {
			buffer->last->next = new;
			new->prev = buffer->last;
			buffer->last = new;
		}
		temp = temp->next;
		buffer->nitems++;
		buffer->size += strlen(new->value);
		count++;
	}
	deleteTB(tb, from, to);
	return buffer;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	if (search == NULL)
		abort();
	Node first = tb->first;
	Match newList, prev;
	newList = NULL;
	int searchLen = strlen(search);
	int found = 0;
	for (int i = 0; i < tb->nitems; i++) {
		char *string = first->value;
		while ((string = strstr(string , search))) {
			Match list = malloc(sizeof(matchNode));
			list->lineNumber = i + 1;
			list->columnNumber = (int)(string - first->value) + 1;
			list->next = NULL;
			string += searchLen;
			if (newList == NULL)
			{
				newList = prev = list; 
			}else {
				prev->next = list;
				prev = list;
			}
		}
		first = first->next;
	}
	
	return newList;
	
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {
	if (to < from) {
		fprintf(stderr, "Invalid arguments please check");
		abort();
	}
	if (from < 1 || to > tb->nitems) {
		fprintf(stderr, "Lines out of range\n");
		abort();
	}
	if (tb->nitems == 0) {
		fprintf(stderr, "Can not delete lines in an empty buffer\n");
		abort();
	}
	int count = 1;
	Node temp = tb->first;
	while (count < from) {
		count++;
		temp = temp->next;
	}
	while (count <= to) {
		if (temp == tb->first)
		{
			Node delete = temp;
			tb->first = temp->next;
			temp = temp->next;
			temp->prev = NULL;
			freeNode(delete);
		}
		else if (temp == tb->last)
		{
			Node delete = temp;
			tb->last = temp->prev;
			temp = temp->prev;
			temp->next = NULL;
			freeNode(delete);
		}
		else
		{
			Node delete = temp;
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			temp = temp->next;
			freeNode(delete);
		}
		tb->nitems--;
		count++;
	}
	
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb) {

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2) {
	return NULL;
}

void undoTB(TB tb) {

}

void redoTB(TB tb) {

}


