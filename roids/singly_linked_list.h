#pragma once
#include <Windows.h>

typedef struct tagSLLNode
{
	void* data;
	struct SLL_Node* next;
}SLL_Node;

extern SLL_Node* root;
extern int nodeCount;

BOOL SLL_Init();
int SLL_ForceNodeCountUpdate();
SLL_Node* SLL_GetNodeAt(int index);
BOOL SLL_AddNode(SLL_Node* node);
BOOL SLL_RemoveNodeAt(int index);