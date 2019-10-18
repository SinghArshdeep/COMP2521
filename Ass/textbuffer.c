
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"
typedef struct TBNode *Node;
// Static function prototypes
static void freeNode(Node curr);
static char *showLineNumber(TB tb);
// static int checkSize(TB tb);

typedef struct TBNode {
	char *value;    // Store the string 
	Node prev; 		// Pointer to previous node 
	Node next; 		// Pointer to next node
} Text;

typedef struct textbuffer {
	int nitems;      /**< count of items in list */
	Node first;  		/**< first node in list */
	Node last;   		/**< last node in list */
	int size;
} Buffer;


/*
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text) {	
	TB buffer = malloc(sizeof(buffer));
	if (buffer == NULL) fprintf(stderr, "couldn't allocate TB node");
	buffer->nitems = 0;
	buffer->size = 0;
	
	// Check if the string is NUll and return empty buffer 
	if (text == NULL)
	{
		buffer->last = buffer->first = NULL;
		return buffer;
	}

	char *string;
	char newLine[1] = "\n";
	if (strcmp(newLine, text) == 0)
	{
		Node new = malloc(sizeof(new));
		new->value = strdup("");
		new->prev = new->next = NULL;
		
		// Link the node to the buffer 
		buffer->first = buffer->last = new;
		buffer->nitems++;
		buffer->size++;
		return buffer;
	}
	char *textTemp = malloc(strlen(text) + 1);
	strcpy(textTemp, text);
	string = strtok(textTemp, newLine);
	buffer->size = strlen(textTemp);
	int count = 0;

	while (string != NULL)
	{
		count++;
		if (count == 1)
		{
			Node new = malloc(sizeof(Node));
			new->value = malloc(sizeof(string));
			new->value = strdup(string);
			// strcpy(new->value, string);
			new->prev = new->next = NULL;
			printf("value is %s \n",string);
			printf("Mallc is %s \n",new->value);

			// Link the node to the buffer 
			buffer->first = buffer->last = new;
		}
		else
		{
			Node temp = malloc(sizeof(temp));
			if (temp == NULL) fprintf(stderr, "couldn't allocate TB node");
			temp->value = strdup (string);
			temp->prev = buffer->last;
			buffer->last->next = temp;
			temp->next = NULL;
			buffer->last = temp;
			printf("value is %s \n",string);
			printf("Mallc is %s \n",temp->value);
		}
		string = strtok(NULL, newLine);
	}

	buffer->nitems = count;
	Node temp = buffer->first;
	while (temp != NULL)
	{
		printf("%s \n", temp->value);
		temp = temp->next;
	}
	return buffer;
}

// A function to count new line characters in a given string 
// static int countNewLine(char *text){
// 	size_t size = strlen(text);
// 	int count = 0;
// 	for (size_t i = 0; i < size; i++)
// 	{
// 		if (text[i] == '\n')
// 		{
// 			count++;
// 		}
// 	}
// 	return count;
// }
/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB(TB tb) {
	if (tb == NULL)
	{
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
static void freeNode(Node curr){
	if (curr == NULL)
	{
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
	if (tb->first == NULL){
		return NULL;
	}
	if (showLineNumbers == true){
		return showLineNumber(tb);
	}
	char *string = malloc(tb->size);
	strcpy(string, tb->first->value);
	// strcat(string, "\n");
	// Node temp = tb->first->next;
	
	// while (temp != NULL)
	// {
	// 	strcat(string, temp->value);
	// 	strcat(string, "\n");
	// 	temp = temp->next;
	// }
	return string;
}

// Returns the size for the string
// static int checkSize(TB tb){
// 	int count = 0;
// 	int n = tb->nitems;
// 	while(n != 0)
//     {
//         n /= 10;
//         ++count;
//     }
// 	n = count + 2;
// 	count = tb->size + (n * sizeof(char));
// 	return count;
// }

// Returns a string with line numbers 
static char *showLineNumber(TB tb){
	return NULL;
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
	return NULL;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	return NULL;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {

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

