
#include <stdlib.h>

#include "tree.h"

int TreeSumOdds(Tree t) {
	if (t == NULL)
	{
		return 0;
	}
	int left = TreeSumOdds(t->left); 
	int right = TreeSumOdds(t->right);
	if (t->value % 2 != 0)
	{
		return t->value + left + right;
	}
	return 0;
}

