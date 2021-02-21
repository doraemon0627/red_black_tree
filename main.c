#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"
#include "red_black_tree.h"

/* ANSI C */

/* black  = 0
red = 1
*/

int main(void)
{
	BTree* tree = MakeBTree();
	BTData foo[100];
	int i = 0;

	for(i = 0; i < 100; i++)
	{
		foo[i].iKey = i;
		foo[i].iValue = i;
	}
	
	for(i = 0; i < 8; i++)
	BTInsert(tree, foo[i]);
	
}
