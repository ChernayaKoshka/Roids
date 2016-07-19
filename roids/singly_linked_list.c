#include "singly_linked_list.h"
#include <assert.h>

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
	int tnodeCount = 0;
	SLL_Node* node = root;
	while (node != NULL)
	{
		if (tnodeCount == index)
			return node;
		node = node->next;
		tnodeCount++;
	}
	return NULL;
}

BOOL SLL_AddNode(SLL_Node* node)
{
	SLL_Node* temp = root;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = node;
	node->prev = temp;
	nodeCount++;
	return TRUE;
}

BOOL SLL_RemoveNodeAt(int index)
{
	nodeCount--;
	SLL_Node* toRemove = root;
	SLL_Node* tempNodeNext;
	SLL_Node* tempNodePrev;

	for (int i = 0; i < index; i++)
		toRemove = toRemove->next;

	tempNodeNext = toRemove->next;
	tempNodePrev = toRemove->prev;

	if (index == 0)
		root = tempNodeNext;

	free(toRemove->data);
	free(toRemove);

	if (tempNodeNext != NULL)
		tempNodeNext->prev = tempNodePrev;

	if (tempNodePrev != NULL)
		tempNodePrev->next = tempNodeNext;

	return TRUE;
}