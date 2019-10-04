#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

void testBefore(DLList L);
void testAfter(DLList L);
void testdelete(DLList L);

int main (void)
{
	printf("Enter input: ");
	DLList myList = getDLList (stdin);
	printf("Program starts!!!!!!!!!!!\n");
	putDLList (stdout, myList);
	assert (validDLList (myList));

	testAfter(myList);
	testBefore(myList);
	testdelete(myList);

	freeDLList (myList);
	return EXIT_SUCCESS;
}

void testAfter(DLList L)
{
	printf("\n ========Testing if DLListAfter works properly======== \n ");
	printf("Currently points at %s and number of elements %zu", DLListCurrent(L), DLListLength(L));
	printf("\n Adding new elements \n");
	char new[10];
	for (int i = 3; i < 7; i++)
	{
		sprintf(new, "%d", i);
		DLListAfter(L , new);
	}
	printf("Print New List !!!!!!!!!\n");
	putDLList (stdout, L);
	assert (validDLList (L));

}

void testBefore(DLList L)
{
	printf("\n ========Testing if DLListBefore works properly======== \n ");
	printf("Currently points at %s and number of elements %zu", DLListCurrent(L), DLListLength(L));
	printf("\n Adding new elements \n");
	char new[10];
	for (int i = 11; i < 17; i++)
	{
		sprintf(new, "%d", i);
		DLListBefore(L , new);
	}
	printf("Print New List !!!!!!!!!\n");
	putDLList (stdout, L);
	assert (validDLList (L));

}

void testdelete(DLList L)
{
	printf("\n ========Testing if DLListDelete works properly======== \n ");
	printf("Currently points at %s and number of elements %zu", DLListCurrent(L), DLListLength(L));

	printf("\n !!!!!!!Deleting current element!!!!!!\n");
	DLListDelete(L);

	printf("\n Print New List !!!!!!!!!\n");
	putDLList (stdout, L);
	assert (validDLList (L));

	printf("\n !!!!!!!Deleting First element!!!!!!\n");
	DLListMoveTo(L, 1);
	printf("Currently points at %s and number of elements %zu", DLListCurrent(L), DLListLength(L));
	DLListDelete(L);

	printf("\n Print New List !!!!!!!!!\n");
	putDLList (stdout, L);
	assert (validDLList (L));

	printf("\n !!!!!!!Deleting Last element!!!!!!\n");
	DLListMoveTo(L, DLListLength(L) + 1);
	printf("Currently points at %s and number of elements %zu", DLListCurrent(L), DLListLength(L));
	DLListDelete(L);

	printf("\n Print New List !!!!!!!!!\n");
	putDLList (stdout, L);
	assert (validDLList (L));


	
}
