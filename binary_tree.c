#include <stdio.h>
#include <stdlib.h>

#include "red_black_tree.h"
#include "binary_tree.h"
#include "user_define.h" /* tree type is defined in here */

BTree *MakeBTree(void)
{
	BTree *pNewTree = 0;
	pNewTree = calloc(1, sizeof(BTree));
	pNewTree->iNumOfNode = 0;
	return pNewTree;
}

BTreeNode *MakeBTreeNode(void)
{
	return calloc(1, sizeof(BTreeNode));
}

BTData GetData(BTreeNode *bt)
{
	return bt->data;
}

void setData(BTreeNode *bt, BTData data)
{
	bt->data = data;
}

BTreeNode *GetLeftSubTree(BTreeNode *bt)
{
	return bt->left;
}

BTreeNode *GetRightSubTree(BTreeNode *bt)
{
	return bt->right;
}

void ChangeLeftSubTree(BTreeNode *dest, BTreeNode *src)
{
	dest->left = src;
}

void ChangeRightSubTree(BTreeNode *dest, BTreeNode *src)
{
	dest->right = src;
}

BTreeNode *GetParents(BTreeNode *bt)
{
	return bt->parents;
}

BTreeNode **GetParentsSpace(BTreeNode *bt)
{
	return &(bt->parents);
}

void BTInsert(BTree *bt, BTData data)
{
	BTreeNode *current_node = bt->root;
	BTreeNode *newnode = calloc(1, sizeof(BTreeNode));

	newnode->data = data;

	if (bt->root == NULL)
	{
		bt->root = newnode;
		*GetParentsSpace(newnode) = NULL; /* if the newnode becomes a root there's no newnode's parents */
#ifdef RED_BLACK_TREE
		ChangeRBTColor(newnode, BLACK);
#endif
		return;
	}

	bt->iNumOfNode++;

	ChangeRBTColor(newnode, RED);

	while (1)
	{
		if (data.iKey < current_node->data.iKey)
		{
			/* input key is lower than current key */
			if (GetLeftSubTree(current_node) == NULL)
			{
				/* input node to currnet's left */ 
				ChangeLeftSubTree(current_node, newnode);
				GetLeftSubTree(current_node)->parents = current_node;
				break;
			}
			else
			{
				/* GetLeftSubTree(current_node)->parents = current_node; */
				current_node = GetLeftSubTree(current_node);
			}
		}
		else if (data.iKey > current_node->data.iKey)
		{
			if (GetRightSubTree(current_node) == NULL)
			{
				ChangeRightSubTree(current_node, newnode);
				GetRightSubTree(current_node)->parents = current_node;
				break;
			}
			else
			{
				/* GetRightSubTree(current_node)->parents = current_node; */
				current_node = GetRightSubTree(current_node);
			}
		}
		else
		{
			fprintf(stderr, "iKey is overlaped\n");
			bt->iNumOfNode;
			free(newnode);
			break;
		}
	}
#ifdef AVL_TREE
	AvlRebalance(bt, bt->root);
#endif

#ifdef RED_BLACK_TREE
	CheckRedBlack(bt, newnode);
#endif
}

BTreeNode *BTSearchNode(BTree *bt, TData iKey)
{
	BTreeNode *current_node = bt->root;
	BTreeNode *found_node = NULL;
	int flag = false; /* true is found, false is not found */

	while (current_node != NULL)
	{
		if (current_node->data.iKey == iKey)
		{
			found_node = current_node;
			flag = true;
			break;
		}
		else if (iKey < current_node->data.iKey)
			current_node = GetLeftSubTree(current_node);
		else if (iKey > current_node->data.iKey)
			current_node = GetRightSubTree(current_node);
	}

	if (flag == false)
	{
		fprintf(stderr, "No data was found");
		return NULL;
	}

	return found_node;
}

#if 0

TData BTSearchData(BTree *bt, TData ikey)
{
	BTreeNode *current_node = bt->root;
	int found_data;
	int flag = false; /* true is found, false is not found */

	while(current_node != NULL)
	{
		if (rootnode == iKey)
		{
			found_data = current_node->data.iValue;
			flag = true;
		}
		else if (iKey > current_node->data.iKey)
			current_node = GetLeftSubTree(current_node);
		else if (iKet < current_node->data.iKey)
			current_node = GetRightSubTree(current_node);
	}

	if (flag = false)
	{
		puts("Data not found");
		return NOT_FOUND;
	}

	return found_data;
}

#endif

TData BTSearchData(BTree *bt, TData iKey)
{
	return BTSearchNode(bt, iKey)->data.iValue;
}

int BTDelete(BTree *bt, TData iKey)
{
	BTreeNode *current_node = NULL, *target_node = NULL;
	target_node = BTSearchNode(bt, iKey);

	if (target_node->left != NULL)
	{
		current_node = target_node ->left;

		while(current_node->right != NULL)
		{
			current_node = current_node->right;
		}

		if (current_node->parents->left == current_node)
			current_node->parents->left = NULL;
		else
			current_node->parents->right = NULL;

		if(target_node != bt->root)
		{
			if (target_node->parents->left == target_node)
				target_node->parents->left = current_node;
			else
				target_node->parents->right = current_node;
		}
		else
		{
			bt->root = current_node;
		}
		current_node->left = target_node->left;
		current_node->right = target_node->right;


	}
	else
	{
		if(target_node != bt->root)
		{
			if (current_node->parents->left == current_node)
				current_node->parents->left = NULL;
			else if	(current_node->parents->right == current_node)
				current_node->parents->right = NULL;
		}
	}

	free(target_node);

	return true;
}


int GetBTreeHeight(BTreeNode *bt, BTree *bT)
{
	int leftH = 0;
	int rightH = 0;

	if (bt == NULL)
		return 0;

	leftH = GetBTreeHeight(GetLeftSubTree(bt), bT);
	rightH = GetBTreeHeight(GetRightSubTree(bt), bT);

	if (leftH > rightH)
		return leftH + 1;
	else
		return rightH + 1;
}
