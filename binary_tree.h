#ifndef BINARY_TREE_C
#define BINARY_TREE_C

#include "user_define.h"

/* BTree = binarytree
BTreeNode = binary tree node
RBT = red-black tree
RBTNode = red-black tree node
*/

#define NOT_FOUND -99999

#define true 1
#define false 0

#define LEFT 0
#define RIGHT 1
#define NONE 2

typedef int TData;

typedef struct _binary_tree_data
{
	TData iKey;
	TData iValue;
}BTData;

#ifndef RED_BLACK_TREE

typedef struct _binary_tree_node
{

	BTData data;

	struct _binary_tree_node *left;
	struct _binary_tree_node *right;
	struct _binary_tree_node *parents;

}BTreeNode;
#endif

#ifdef RED_BLACK_TREE

#ifndef RED
#define RED 0
#endif

#ifndef BLACK
#define BLACK 1
#endif

typedef struct _binary_tree_node
{
	BTData data;

	struct _binary_tree_node *left;
	struct _binary_tree_node *right;
	struct _binary_tree_node *parents;

	RBTCOLOR color;
}BTreeNode;

#endif

typedef struct _binary_tree
{
	struct _binary_tree_node *root;
	int iNumOfNode;
}BTree;

BTree *MakeBTree (void);
BTreeNode *MakeBTreeNode (void);


BTData GetData(BTreeNode *bt);
void setData(BTreeNode *bt, BTData data);


BTreeNode *GetLeftSubTree (BTreeNode *bt);
BTreeNode *GetRightSubTree (BTreeNode *bt);


BTreeNode *GetParents (BTreeNode *bt);
BTreeNode **GetParentsSpace (BTreeNode *bt);


void ChangeLeftSubTree(BTreeNode *dest, BTreeNode *src);
void ChangeRightSubTree(BTreeNode *dest, BTreeNode *src);
void BTInsert(BTree *bt, BTData data);


BTreeNode *BTSearchNode (BTree *bt, TData ikey);
TData BTSearchData(BTree *bt, TData ikey);


int BTDelete(BTree *bt, TData iKey);
int GetBTreeHeight(BTreeNode *bt, BTree *bT);




#endif
