
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
static void checkHash(char *string, TB tb, Node curr);
static void checkBold(char *string, TB tb, Node curr);
/*
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text) {
	// Initialising the new buffer 
	TB buffer = malloc(sizeof(struct textbuffer));
	if (buffer == NULL) {
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	if (text != NULL) {
		buffer->size = strlen(text);
	}else {
		buffer->size = 0;
	}

	buffer->nitems = 0;
	buffer->first = buffer->last = NULL;

	// if no string is passed return an empty buffer
	if (text == NULL) {
		return buffer;
	}
	char *textTemp = malloc(strlen(text)*sizeof(char) + 1);
	if (textTemp == NULL){
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	strcpy(textTemp, text);

	char *string = strchr(textTemp , '\n');
	char *data = textTemp;
	// Loop to go through the whole string 
	while (string != NULL) {
		string[0] = '\0';
		Node new = malloc(sizeof(struct TBNode)*sizeof(char));
		if (new == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		new->value = malloc(strlen(data)*sizeof(char) + 1);
		new->next = new->prev = NULL;
		if (new->value == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		strcpy(new->value, data);
		// Add each node to the buffer 
		if (buffer->last == NULL) {
			buffer->first = buffer->last = new;
		}else {
			buffer->last->next = new;
			new->prev = buffer->last;
			buffer->last = new;
		}
		data = &string[1];
		string = strchr(data , '\n');
		buffer->nitems++;
	}
	// Free the external memory used 
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
	// Loop through the whole buffer
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
	// Free the string and the node 
	free(curr->value);
	free(curr);
}
/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB(TB tb, bool showLineNumbers) {
	// Return NULL if buffer is empty 
	if (tb->first == NULL) {
		return NULL;
	}
	// Pass to the function if numbers are needed
	if (showLineNumbers == true) {
		return showLineNumber(tb);
	}
	char *string = calloc(1, (tb->size)*sizeof(char) + 3);
	if (string == NULL) {
		fprintf(stderr, "Error in allocating memory for buffer");
		abort();
	}
	Node temp = tb->first;
	// Catenate all the strings into one string in a loop 
	while (temp != NULL) {
		strcat(string, temp->value);
		strcat(string, "\n");
		temp = temp->next;
	}
	// Return the string 
	return string;
}

// Returns the size needed for a string in accordance to the number of digits 
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

	// Catenate all the strings into one string in a loop 
	while (temp != NULL) {
		count++;
		// Print the numbers as well 
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
	// Go to the node 
	while (count < from ) {
		count++;
		temp = temp->next;
	}
	// Catenate all the strings into one string in a loop 
	while (count <= to) {
		count++;
		char *string = calloc(1, (2 + strlen(temp->value) + strlen(prefix)) * sizeof(char));
		// Add the prefix to the string first 
		strcpy(string, prefix);
		strcat(string, temp->value);
		free(temp->value);
		temp->value = string;
		temp = temp->next;
	}
	// Size if added for the string and the null characters 
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
	// if tb1 is empty then return tb2 as tb1 
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
	// Gets the previous node from the position 
	Node temp = tb1->first;
	while (count < (pos - 1)) {
		count++;
		temp = temp->next;
	} 
	// Add nodes of tb2 to tb1 at the proper position 
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
	// Update the size and number of items 
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
	char *text = dumpTB(tb2, false);
	TB tb3 = newTB(text);
	mergeTB(tb1, pos, tb3);
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
	// Check for valid range 
	if (to < from)
		return NULL;
	if (from < 1 || to > tb->nitems) {
		fprintf(stderr, "Lines out of range\n");
		abort();
	}
	// Check for empty buffer 
	if (tb->nitems == 0) {
		fprintf(stderr, "Can not delete lines in an empty buffer\n");
		abort();
	}
	// Allocate memory for new buffer 
	TB buffer = malloc(sizeof(struct textbuffer));
	if (buffer == NULL) {
		fprintf(stderr, "Error in allocating memory\n");
		abort();
	}
	// Initialize the buffer 
	buffer->nitems = 0;
	buffer->size = 0;
	buffer->first = buffer->last = NULL;
	int count = 1;
	Node temp = tb->first;
	// Find the node at the position 
	while (count < from) {
		count++;
		temp = temp->next;
	}
	// Loop through the list to find the deleting nodes 
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
		// Find the correct position in the buffer 
		if (buffer->last == NULL) {
			buffer->first = buffer->last = new;
		}else {
			buffer->last->next = new;
			new->prev = buffer->last;
			buffer->last = new;
		}
		// Update the pointer and the items in the buffer 
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
	// Loop through all the lines in the buffer 
	for (int i = 0; i < tb->nitems; i++) {
		char *string = first->value;
		// Check for occurences in a single string 
		while ((string = strstr(string , search))) {
			// Allocate memory for new node 
			Match list = malloc(sizeof(matchNode));
			if (list == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
			}
			// Calculate values for the items in struct match 
			list->lineNumber = i + 1;
			list->columnNumber = (int)(string - first->value) + 1;
			list->next = NULL;
			string += searchLen;
			// Add to the list 
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
	// Check for valid range 
	if (to < from) {
		fprintf(stderr, "Invalid arguments please check");
		abort();
	}
	if (from < 1 || to > tb->nitems) {
		fprintf(stderr, "Lines out of range\n");
		abort();
	}
	// Check for empty buffer 
	if (tb->nitems == 0) {
		fprintf(stderr, "Can not delete lines in an empty buffer\n");
		abort();
	}
	int count = 1;
	Node temp = tb->first;
	// Find the node at the correct position 
	while (count < from) {
		count++;
		temp = temp->next;
	}
	// Loop after the position node 
	while (count <= to) {
		// Check for the placement for each node 
		// Execute code for deleting the node 
		if (temp == tb->first) {
			Node delete = temp;
			tb->first = temp->next;
			temp = temp->next;
			temp->prev = NULL;
			freeNode(delete);
		}else if (temp == tb->last) {
			Node delete = temp;
			tb->last = temp->prev;
			temp = temp->prev;
			temp->next = NULL;
			freeNode(delete);
		}else {
			Node delete = temp;
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			temp = temp->next;
			freeNode(delete);
		}
		// Update the number of items in the buffer
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
	Node curr = tb->first;
	// Loop through all the lines in the buffer 
	while (curr != NULL)
	{
		checkHash(curr->value, tb, curr);
		// checkBold(curr->value, tb, curr);
		curr = curr->next;
	}
	
}

// Function to check the occurence of hash in the given line 
// Execute code if hash if found 
static void checkHash(char *string, TB tb, Node curr) {
	// Check if a hash is present 
	if (strncmp(string, "#", 1) == 0)
	{
		Node new = curr;
		// Allocate new memory for the line
		new->value = calloc(1, (strlen(string) + 10)*sizeof(char));
		if (new->value == NULL) {
			fprintf(stderr, "Error in allocating memory for buffer");
			abort();
		}
		// Catenate strings to the new string 
		tb->size += 10;
		strcpy(new->value, "<h1>");
		string++;
		strcat(new->value, string);
		strcat(new->value, "</h1>");
		// Free memory for old string 
		free(curr->value);
		curr->value = new->value;
	}
	return;
}


static void checkBold(char *string, TB tb, Node curr) {
	
	// char *text = malloc(strlen(string)*sizeof(char) + 1);
	// strcpy(text, string);
	// char *new = strchr(text, '*');
	// char *textTemp = text;
	// char *prev, *next, *temp;
	// int count, beforeLen;
	// printf("reches afes");
	// while (new != NULL && strlen(string) > 2)
	// {
	// 	printf("reches here");
	// 	prev = new;
	// 	textTemp = &new[1];
	// 	new = strchr(textTemp, '*');
	// 	int count = (int)(new - prev);
	// 	if (count > 0)
	// 	{
	// 		temp = prev;
	// 		temp++;
	// 		beforeLen = (int)(prev - string);
	// 		Node copy = curr;
	// 		copy->value = calloc(1, (strlen(string) + 6)*sizeof(char));
	// 		tb->size += 6;
	// 		strncpy(copy->value, string, beforeLen);
	// 		strcat(copy->value, "<i>");
	// 		strncat(copy->value, temp, count);
	// 		free(curr->value);
	// 		curr->value = copy->value;
	// 		textTemp = &new[1];
	// 		new = strchr(textTemp, '*');
	// 	}else
	// 	{
	// 		continue;
	// 	}
		
	// }
	
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


