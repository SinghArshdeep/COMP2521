// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

static void testNewTB(void);
static void show(TB tb);
static void testMergeTB(void);
static void testPasteTB(void);
static void testCutTB(void);
static void testSearchTB(void);

// TODO: Add more function prototypes

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


int main(void) {
	
	// testNewTB();
	// testMergeTB();
	// testPasteTB();
	//testCutTB();
	testSearchTB();
	// TODO: Call more test functions
	
	printf("All tests passed! I'm AWESOME!\n");
}

static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");

	// Calling dumpTB immediately after newTB, without modifying the TB
	printf("TEST 1: The Given String \n\n");
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");

	assert(linesTB(tb1) == 4);

	// printf("TEST 2: Checking for an empty line \n\n");
	// TB tb2 = newTB("\n");
	// // printf("Number of line is %d", linesTB(tb2));
	// assert(linesTB(tb2) == 1);

	// printf("TEST 3: Checking for NULL line \n\n");
	// TB tb3 = newTB(NULL);
	// assert(linesTB(tb3) == 0);

	printf("TEST 4: Testing dumpTB without numbers \n\n");
	char *text1 = dumpTB(tb1, false); // Don't show line numbers
	// printf("%s", text1);
	assert(strcmp("hello there,\nhow\nare\nthings\n", text1) == 0);

	printf("TEST 5: Testing dumbTB with numbers \n\n");
	char *text2 = dumpTB(tb1, true); // Show line numbers
	// printf("%s", text2);
	assert(strcmp("1. hello there,\n2. how\n3. are\n4. things\n", text2) == 0);

	free(text1);
	free(text2);

	printf("TEST 5: VISUAL test after adding prefix  \n\n");
	addPrefixTB(tb1, 1, 4, "New ");
	show(tb1);

	releaseTB(tb1);
	// releaseTB(tb3);
	
	printf("newTB tests passed!\n\n");
}

static void show(TB tb){
	Node temp = tb->first;
	while (temp != NULL)
	{
		printf("%s\n", temp->value);
		temp = temp->next;
	}
}


// TODO: Add more test functions
static void testMergeTB(void) {
	
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");
	assert(linesTB(tb1) == 4);

	TB tb3 = newTB("I'm\nvery good\n");
	assert(linesTB(tb3) == 2);

	mergeTB(tb1, 5, tb3);
	
	assert(linesTB(tb1) == 6);
	printf("\nTEST: VISUAL test after Merging \n\n");
	show(tb1);

	releaseTB(tb1);
}

static void testPasteTB(void) {
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");
	assert(linesTB(tb1) == 4);

	TB tb3 = newTB("I'm\nvery good\n");
	assert(linesTB(tb3) == 2);

	printf("\nTEST: VISUAL test after Pasting  \n\n");
	pasteTB(tb1, 5, tb3);
	show(tb1);

	assert(linesTB(tb1) == 6);
	printf("\nChecking if we can still access the pasted buffer  \n\n");
	show(tb3);
	releaseTB(tb1);
	releaseTB(tb3);
}

static void testCutTB(void) {
	printf("\nTESTING cutTB Function \n");
	TB tb1 = newTB("hello there,\nhow\nare\nthings!\nTell\nme\n");
	assert(linesTB(tb1) == 6);
	show(tb1);
	TB tb = cutTB(tb1, 3, 5);
	printf("\nTEST: VISUAL test after cut  \n\n");
	show(tb1);
	printf("\nTEST: Calling dumstring  \n\n");
	char *text = dumpTB(tb, true);
	printf("%s\n", text);
	free(text);
}

static void testSearchTB(void){
	TB tb1 = newTB("abracadabra alacazam\nabracadabracadabracadabracadabra\n");
	char *str = "abracadabra";
	Match list = searchTB(tb1, str);
	Match curr = list;
	while(curr != NULL){
		printf("%d %d\n" , curr->lineNumber , curr->columnNumber);
		curr = curr->next;
	}
	releaseTB(tb1);
	while(list != NULL){
	Match temp = list;
	list = list->next;
	free(temp);
	}
}
