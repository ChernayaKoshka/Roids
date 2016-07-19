#pragma once
#include <Windows.h>

typedef struct tagDLLNode
{
	void* data;
	struct DLL_Node* prev;
	struct DLL_Node* next;
}DLL_Node;

extern DLL_Node* root;
extern int nodeCount;

BOOL DLL_Init();
int DLL_ForceNodeCountUpdate();
DLL_Node* DLL_GetNodeAt(int index);
BOOL DLL_AddNode(DLL_Node* node);
BOOL DLL_RemoveNodeAt(int index);