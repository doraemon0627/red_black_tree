#include <stdio.h>
#include <stdlib.h>

#include "red_black_tree.h"

void ChangeRBTColor(BTreeNode *dest, RBTCOLOR color)
{
	dest->color = color;
}

RBTCOLOR GetRBTNodeColor(BTreeNode *node)
{
	return node->color;
}

bool RestructureOrRecolor(BTreeNode *node)
{
	bool restructure_flag = (bool)FALSE;
	bool recolor_flag = (bool)FALSE;
	bool is_w_red = (bool)FALSE;
	bool is_w_black = (bool)FALSE;
	bool result = (bool)FALSE;

	BTreeNode *u = NULL;

	u = GetParents(GetParents(node));

	if (GetRightSubTree(u) == GetParents(node))
	{
		if (GetLeftSubTree(u) == NULL) 
			is_w_black = (bool)TRUE;
		else
			if (GetRBTNodeColor(GetLeftSubTree(u)) == BLACK)
				is_w_black = (bool)TRUE;

		if (GetLeftSubTree(u) != NULL) 
			if (GetRBTNodeColor(GetLeftSubTree(u)) == RED)
				is_w_red = (bool)TRUE;

		if(is_w_red)
			recolor_flag = (bool)TRUE;
		if (is_w_black)
			restructure_flag = (bool)TRUE;
	}
	else if (GetLeftSubTree(u) == GetParents(node))
	{
		if (GetRightSubTree(u) == NULL) 
			is_w_black = (bool)TRUE;
		else
			if (GetRBTNodeColor(GetRightSubTree(u)) == BLACK)
				is_w_black = (bool)TRUE;

		if (GetRightSubTree(u) != NULL) 
			if (GetRBTNodeColor(GetRightSubTree(u)) == RED)
				is_w_red = (bool)TRUE;

		if(is_w_red)
			recolor_flag = (bool)TRUE;
		if (is_w_black)
			restructure_flag = (bool)TRUE;
	}

	if (restructure_flag == recolor_flag)
	{
		fprintf(stderr, "exception : restructure_flag and recolor_flag are same\n");
		exit(EXIT_FAILURE);
	}

	if (restructure_flag == (bool)TRUE)
		result = (bool)RESTRUCTURE;
	else if (recolor_flag == (bool)TRUE)
		result = (bool)RECOLOR;
	else
	{
		fprintf(stderr, "exception : behavior is not seleted(restructuring or recoloring) \n");
		exit(EXIT_FAILURE);
	}

	return result;
}

bool IsDoubleRed(BTreeNode *node)
{
	bool result = (bool)FALSE;

	if (GetParents(node) != NULL)
		if (GetRBTNodeColor(node) == RED && 
			GetRBTNodeColor(GetParents(node)) == RED)
			result = (bool)TRUE;

	return result;
}

void Restructure(BTree *bt, BTreeNode *node)
{
	/* final tree form

	*tree configuration*

	-------b4(It was r4 before restructuring)
	------/--\
	-----r2---r6
	----/-\---/-\
	---#---#--#-#
	*/

	BTreeNode *tempnode = NULL; 
	/*Tempnod will be parents node of the sub-tree if node is not a root of the bt.
	If node is a root of bt, tempnode will just be null*/
	BTreeNode *u = NULL;
	BTreeNode *temp_u_parents = NULL; 
	BTreeNode *temp_sub_tree1 = NULL, *temp_sub_tree2 = NULL;
	bool side = 0;

	u = GetParents(GetParents(node));

	if (u != bt->root)
		tempnode = GetParents(node);

	temp_u_parents = GetParents(u);

	if (tempnode != NULL)
	{
		if (GetRightSubTree(temp_u_parents) == u)
			side = RIGHT;
		else if (GetLeftSubTree(temp_u_parents) == u)
			side = LEFT;
		else
		{
			fprintf(stderr, "exception R1: parents and child doens't match\n");
			exit(EXIT_FAILURE);
		}
	}
	if (GetRightSubTree(u) == GetParents(node))
	{
		if (GetLeftSubTree(GetParents(node)) == node)
		{	
			/* case 1 */

			/* tree configuration
			------------b3(u)
			------------/-\
			-----------#---r6
			--------------/-\
			-------(node)r4-#
			-------------/-\  
			------------#---#
			*/

			temp_sub_tree1 = GetLeftSubTree(node);
			temp_sub_tree2 = GetRightSubTree(node);

			ChangeLeftSubTree(node, GetParents(GetParents(node)));
			ChangeRightSubTree(node, GetParents(node));

			ChangeRBTColor(node, BLACK);
			ChangeRBTColor(GetLeftSubTree(node), RED);

			ChangeRightSubTree(GetLeftSubTree(node), temp_sub_tree1);
			ChangeLeftSubTree(GetRightSubTree(node), temp_sub_tree2);

			if (temp_sub_tree1 != NULL)
				*GetParentsSpace(temp_sub_tree1) = GetLeftSubTree(node);
			if (temp_sub_tree2 != NULL)
				*GetParentsSpace(temp_sub_tree2) = GetRightSubTree(node);

			*GetParentsSpace(GetLeftSubTree(node)) = node;
			*GetParentsSpace(GetRightSubTree(node)) = node;

			if (tempnode != NULL)
			{
				*GetParentsSpace(node) = temp_u_parents;

				if (side == LEFT)
					ChangeLeftSubTree(temp_u_parents, node);
				else
					ChangeRightSubTree(temp_u_parents, node);
			}
			else
			{
				bt->root = node;
				*GetParentsSpace(node) = NULL; /* because there's no parents */
			}
		}
		else if (GetRightSubTree(GetParents(node)) == node)
		{ 
			/* case 2 */

			/* tree configuration
			------------b3(u)
			-------------/-\
			-----------#---r4(p)
			---------------/-\
			---------------#--r6(node)
			-----------------/-\  
			----------------#---#
			*/

			BTreeNode *p;

			p = GetParents(node);

			temp_sub_tree1 = GetLeftSubTree(p);

			ChangeLeftSubTree(p, GetParents(p));
			ChangeRightSubTree(p, GetRightSubTree(p));

			*GetParentsSpace(GetLeftSubTree(p)) = p;
			*GetParentsSpace(GetRightSubTree(p)) = p;

			/* change node's right sub tree to temp_sub_tree1 */
			ChangeRightSubTree(GetLeftSubTree(p), temp_sub_tree1);

			ChangeRBTColor(p, BLACK);
			ChangeRBTColor(GetLeftSubTree(p), RED);

			if (tempnode != NULL)
			{
				*GetParentsSpace(p) = temp_u_parents;

				if (side == LEFT)
					ChangeLeftSubTree(temp_u_parents, p);
				else
					ChangeRightSubTree(temp_u_parents, p);
			}
			else
			{
				bt->root = p;
				*GetParentsSpace(p) = NULL; /* because there's no parents */
			}
		}
	}
	else if (GetLeftSubTree(u) == GetParents(node))
	{
		if (GetRightSubTree(GetParents(node)) == node)
		{
			/* case 3 */

			/* tree configuration
			------------b6(u)
			------------/-\
			-----------r3--#
			-----------/-\ 
			-----------#--r4(node)  
			-------------/-\  
			------------#---#
			*/	

			temp_sub_tree1 = GetLeftSubTree(node);
			temp_sub_tree2 = GetRightSubTree(node);

			ChangeLeftSubTree(node, GetParents(node));
			ChangeRightSubTree(node, GetParents(GetParents(node)));

			ChangeRBTColor(node, BLACK);
			ChangeRBTColor(GetRightSubTree(node), RED);

			ChangeRightSubTree(GetLeftSubTree(node), temp_sub_tree1);
			ChangeLeftSubTree(GetRightSubTree(node), temp_sub_tree2);

			if (temp_sub_tree1 != NULL)
				*GetParentsSpace(temp_sub_tree1) = GetLeftSubTree(node);
			if (temp_sub_tree2 != NULL)
				*GetParentsSpace(temp_sub_tree2) = GetRightSubTree(node);

			*GetParentsSpace(GetLeftSubTree(node)) = node;
			*GetParentsSpace(GetRightSubTree(node)) = node;

			if (tempnode != NULL)
			{
				*GetParentsSpace(node) = temp_u_parents;

				if (side == LEFT)
					ChangeLeftSubTree(temp_u_parents, node);
				else
					ChangeRightSubTree(temp_u_parents, node);
			}
			else
			{
				bt->root = node;
				*GetParentsSpace(node) = NULL; /* because there's no parents */
			}
		}
		else if (GetLeftSubTree(GetParents(node)) == node)
		{
			/* case 4 */

			/* tree configuration
			----------------b6(u)
			----------------/-\
			-------------(p)r4--#
			---------------/--\ 
			--------(node)r3--#
			-------------/-\  
			------------#---#
			*/	

			BTreeNode *p;

			p = GetParents(node);

			temp_sub_tree1 = GetRightSubTree(p);

			ChangeLeftSubTree(p, GetLeftSubTree(p));
			ChangeRightSubTree(p, GetParents(p));

			*GetParentsSpace(GetLeftSubTree(p)) = p;
			*GetParentsSpace(GetRightSubTree(p)) = p;

			/* change node's right sub tree to temp_sub_tree1 */
			ChangeLeftSubTree(GetRightSubTree(p), temp_sub_tree1);

			ChangeRBTColor(p, BLACK);
			ChangeRBTColor(GetRightSubTree(p), RED);

			if (tempnode != NULL)
			{
				*GetParentsSpace(p) = temp_u_parents;

				if (side == LEFT)
					ChangeLeftSubTree(temp_u_parents, p);
				else
					ChangeRightSubTree(temp_u_parents, p);
			}
			else
			{
				bt->root = p;
				*GetParentsSpace(p) = NULL; /* because there's no parents */
			}
		}
	}
}

BTreeNode *Recolor(BTree *bt, BTreeNode *node)
{
	/* recolor case tree configuration

	-----b6(u)
	----/-\
	---r3--r4
	*/ 
	BTreeNode *u = GetParents(GetParents(node));

	if (u != bt->root)
		ChangeRBTColor(u, RED); /* if parents' of u is root u's color won't change */

	ChangeRBTColor(GetLeftSubTree(u), BLACK);
	ChangeRBTColor(GetRightSubTree(u), BLACK);

	return u;
}

void CheckRedBlack(BTree *bt, BTreeNode *node)
{
	BTreeNode *current_node = NULL;

	current_node = node;

	while(IsDoubleRed(current_node))
	{ /* if the sub-tree is double red */
		if (RestructureOrRecolor(current_node) == RESTRUCTURE) /* if the sub-tree has to be restructured */
		{
			Restructure(bt, current_node);
			break;
		}
		else /* if the sub-tree has to be recolored */
		{
			current_node = Recolor(bt, current_node);
		}
	}
}
