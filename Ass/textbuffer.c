
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
// static char *showLineNumber(TB tb);
// static int checkSize(TB tb);


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
	char *textTemp = malloc(strlen(text)*sizeof(char));
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
		new->value = malloc(strlen(string)*sizeof(char));
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
	return buffer;
}
// {
// 	TB buffer = malloc(sizeof(buffer));
// 	if (buffer == NULL) fprintf(stderr, "couldn't allocate TB node");
// 	buffer->nitems = 0;
// 	buffer->size = 0;

// 	// Check if the string is NUll and return empty buffer
// 	if (text == NULL)
// 	{
// 		buffer->last = buffer->first = NULL;
// 		return buffer;
// 	}

// 	char *string;
// 	char newLine[1] = "\n";
// 	if (strcmp(newLine, text) == 0)
// 	{
// 		Node new = malloc(sizeof(new));
// 		new->value = strdup("");
// 		new->prev = new->next = NULL;

// 		// Link the node to the buffer
// 		buffer->first = buffer->last = new;
// 		buffer->nitems++;
// 		buffer->size++;
// 		return buffer;
// 	}
// 	char *textTemp = malloc(strlen(text) + 1);
// 	strcpy(textTemp, text);
// 	string = strtok(textTemp, newLine);
// 	buffer->size = strlen(textTemp);
// 	int count = 0;

// 	while (string != NULL)
// 	{
// 		count++;
// 		if (count == 1)
// 		{
// 			Node new = malloc(sizeof(Node));
// 			new->value = malloc(sizeof(string));
// 			new->value = strdup(string);
// 			// strcpy(new->value, string);
// 			new->prev = new->next = NULL;
// 			printf("value is %s \n",string);
// 			printf("Mallc is %s \n",new->value);

// 			// Link the node to the buffer
// 			buffer->first = buffer->last = new;
// 		}
// 		else
// 		{
// 			Node temp = malloc(sizeof(temp));
// 			if (temp == NULL) fprintf(stderr, "couldn't allocate TB node");
// 			temp->value = strdup (string);
// 			temp->prev = buffer->last;
// 			buffer->last->next = temp;
// 			temp->next = NULL;
// 			buffer->last = temp;
// 			printf("value is %s \n",string);
// 			printf("Mallc is %s \n",temp->value);
// 		}
// 		string = strtok(NULL, newLine);
// 	}

// 	buffer->nitems = count;
// 	Node temp = buffer->first;
// 	while (temp != NULL)
// 	{
// 		printf("%s \n", temp->value);
// 		temp = temp->next;
// 	}
// 	return buffer;
// }

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
	// if (tb->first == NULL) {
	// 	return NULL;
	// }
	// if (showLineNumbers == true) {
	// 	return showLineNumber(tb);
	// }
	// char *string = malloc((tb->size)*sizeof(char));
	// strcpy(string, tb->first->value);
	// strcat(string, "\n");
	// Node temp = tb->first->next;

	// while (temp != NULL) {
	// 	strcat(string, temp->value);
	// 	strcat(string, "\n");
	// 	temp = temp->next;
	// }

	// return string;
	return NULL;
}

// Returns the size for the string
// static int checkSize(TB tb) {
// 	int count = 0;
// 	int n = tb->nitems;
// 	while(n != 0) {
//         n /= 10;
//         ++count;
//     }
// 	n = count + 2;
// 	count = (tb->size + n);
// 	return count;
// }

// Returns a string with line numbers
// static char *showLineNumber(TB tb) {
// 	char *numString = malloc(checkSize(tb)*sizeof(char));
// 	int count = 1;
// 	char *num = NULL;
// 	char text[3];
// 	sprintf(text, "%d", count);
// 	num = text;
// 	strcpy(numString, num);
// 	strcat(numString, ". ");
// 	strcat(numString, tb->first->value);
// 	strcat(numString, "\n");
// 	Node temp = tb->first->next;

// 	while (temp != NULL) {
// 		count++;
// 		sprintf(text, "%d", count);
// 		num = text;
// 		strcat(numString, num);
// 		strcat(numString, ". ");
// 		strcat(numString, temp->value);
// 		strcat(numString, "\n");
// 		temp = temp->next;
// 	}
// 	return numString;
// }

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
	// if (from < 1 || to > linesTB(tb)) {
	// 	fprintf(stderr, "Lines out of range");
	// 	abort();
	// }else if (from > to) {
	// 	fprintf(stderr, "Incorrect line range");
	// 	abort();
	// }
	// int count = 1;
	// Node temp = tb->first;
	// while (count < from ) {
	// 	count++;
	// 	temp = temp->next;
	// }
	
	// while (count <= to) {
	// 	count++;
	// 	char *string = malloc((strlen(temp->value) + strlen(prefix))*sizeof(char));
	// 	strcpy(string, prefix);
	// 	strcat(string, temp->value);
	// 	free(temp->value);
	// 	temp->value = string;
	// 	temp = temp->next;
	// }
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
	// if (pos > linesTB(tb1)) {
	// 	fprintf(stderr, "Line out of range");
	// 	abort();
	// }
	// int count = 1;
	// Node temp = tb1->first;
	// while (count < (pos - 1) ) 
	// {
	// 	count++;
	// 	temp = temp->next;
	// }

	// if (temp == tb1->first && pos == 1) 
	// {
	// 	tb1->first = tb2->first;
	// 	tb2->last->next = temp;
	// 	temp->prev = tb2->last;
	// } else
	// {
	// 	Node new = temp->next;
	// 	temp->next = tb2->first;
	// 	tb2->first->prev = temp;
	// 	tb2->last->next = new;
	// 	new->prev = tb2->last;
	// }

	// tb1->nitems += tb2->nitems;
	// free(tb2);
	
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


