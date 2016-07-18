#include "singly_linked_list.h"

SLL_Node* root;
int nodeCount = 1;

BOOL SLL_Init()
{
	root = calloc(1, sizeof(SLL_Node));
	if (root == NULL)
		return FALSE;
	return TRUE;
}

int SLL_ForceNodeCountUpdate()
{
	int nodeCount = 0;
	SLL_Node* node = root;
	do
	{
		nodeCount++;
		node = node->next;
	} while (node->next != NULL);
	return nodeCount;
}

SLL_Node* SLL_GetNodeAt(int index)
{
	int nodeCount = 0;
	SLL_Node* node = root;
	do
	{
		if (nodeCount == index)
			return node;
		node = node->next;
		nodeCount++;
	} while (node != NULL);
	return NULL;
}

BOOL SLL_AddNode(SLL_Node* node)
{
	SLL_Node* temp = root;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = node;
	nodeCount++;
	return TRUE;
}

BOOL SLL_RemoveNodeAt(int index)
{
	int nodeCount = 0;
	SLL_Node* node = root;
	for (int i = 0; i < index; i++)
	{
		node = node->next;
		if (!node) return FALSE;
	}
	free(node->next);
	node->next = NULL;
	return TRUE;
}