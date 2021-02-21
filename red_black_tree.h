#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "user_define.h"
#include "binary_tree.h"

#define RED 0
#define BLACK 1

#define RESTRUCTURE 0
#define RECOLOR 1

RBTCOLOR GetRBTNodeColor(BTreeNode *bt);
/* checks red-black tree and
recolor or restructure the tree if it has to be */
void CheckRedBlack(BTree *bt, BTreeNode *node); 

int RestructuringCase(BTreeNode *node);
void ChangeRBTColor(BTreeNode *dest, RBTCOLOR color);

#endif RED_BLACK_TREE_H
