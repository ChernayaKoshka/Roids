#include "doubly_linked_list.h"
#include <assert.h>

DLL_Node* root;
int nodeCount = 1;

BOOL DLL_Init()
{
	root = calloc(1, sizeof(DLL_Node));
	if (root == NULL)
		return FALSE;
	return TRUE;
}

int DLL_ForceNodeCountUpdate()
{
	int nodeCount = 0;
	DLL_Node* node = root;
	do
	{
		nodeCount++;
		node = node->next;
	} while (node->next != NULL);
	return nodeCount;
}

DLL_Node* DLL_GetNodeAt(int index)
{
	int tnodeCount = 0;
	DLL_Node* node = root;
	while (node != NULL)
	{
		if (tnodeCount == index)
			return node;
		node = node->next;
		tnodeCount++;
	}
	return NULL;
}

BOOL DLL_AddNode(DLL_Node* node)
{
	DLL_Node* temp = root;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = node;
	node->prev = temp;
	nodeCount++;
	return TRUE;
}

BOOL DLL_RemoveNodeAt(int index)
{
	nodeCount--;
	DLL_Node* toRemove = root;
	DLL_Node* tempNodeNext;
	DLL_Node* tempNodePrev;

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