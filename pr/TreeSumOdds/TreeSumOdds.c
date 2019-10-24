
#include <stdlib.h>

#include "tree.h"

int TreeSumOdds(Tree t) {
	if (t == NULL)
	{
		return 0;
	}else if (t->value % 2 != 0)
	{
		return t->value;
	}else
	{
		return 0;
	}
	return TreeSumOdds(t->left) + TreeSumOdds(t->right);
}

