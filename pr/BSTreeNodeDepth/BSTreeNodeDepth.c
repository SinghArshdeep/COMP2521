
#include <stdlib.h>

#include "BSTree.h"

int BSTreeNodeDepth(BSTree t, int key) {
	if (t == NULL)
	{
		return -1;
	}else if (t->value == key)
	{
		return 0;
	}
	int left = BSTreeNodeDepth(t->left, key);
	int right = BSTreeNodeDepth(t->right, key);
	if (left != -1)
	{
		return 1 + left;
	}else if (right != -1)
	{
		return 1 + right;
	}
	
	return -1;

}

