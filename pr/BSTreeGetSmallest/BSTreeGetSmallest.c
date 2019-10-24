
#include "BSTree.h"

#include <stdlib.h>

BSTree BSTreeGetSmallest(BSTree t) {
	if (t == NULL)
	{
		return NULL;
	}else if (t->left == NULL)
	{
		return t;
	}
		return BSTreeGetSmallest(t->left);
}

