#ifndef BT_H
#define BT_H


#include <stdio.h>

#define MAX_KEYS (8)
extern const int AVG_KEY_INDEX;

typedef int (*bt_compareCallback)(void *a, void *b);
typedef void (*bt_freeCallback)(void *a);
typedef const char *(*bt_toStrCallback)(void *a);

typedef struct _struct_bt_Node
{
	int isLeaf;
	int numKeys;
	void *keys[MAX_KEYS];
	struct _struct_bt_Node *children[MAX_KEYS+1];
} bt_Node;

typedef struct _struct_bt_Tree
{
	bt_Node *root;
	bt_compareCallback cmpCb;
	bt_freeCallback freeCb;
	size_t typeSize;
} bt_Tree;

void bt_valueArrayOpenGap(void *arr[], int s, int index);
void bt_valueArrayCloseGap(void *arr[], int s, int index);
int bt_valueArrayInsert(void *arr[], int s, int n, void *value, bt_compareCallback cmp);
void bt_valueArrayPrint(int arr[], int n);

void bt_nodeArrayOpenGap(bt_Node *arr[], int n, int index);
void bt_nodeArrayCloseGap(bt_Node *arr[], int n, int index);
void bt_nodeArrayPrint(bt_Node *arr[], int n);

bt_Tree *bt_newTree(bt_compareCallback cmpCb, bt_freeCallback freeCb);
bt_Node *bt_newNode(int isLeaf);
void bt_free(bt_Tree *tree);
void bt_freeAux(bt_Tree *tree, bt_Node *root);
void bt_insert(bt_Tree *tree, void *value);
void bt_insertNonRoot(bt_Tree *tree, bt_Node *root, void *value);
void bt_splitChild(bt_Tree *tree, bt_Node *root, int index);
void bt_fprintNode(bt_Node *root, FILE *fp, bt_toStrCallback);
#define bt_printNode(root, toStr) bt_fprintNode(root, stdout, toStr)
void bt_remove(bt_Tree *tree, void *value);
void bt_removeNonRoot(bt_Tree *tree, bt_Node *root, void *value);
void bt_removeFromNonLeaf(bt_Tree *tree, bt_Node *root, int idx);
void *bt_getPred(bt_Node *root, int idx);
void *bt_getSucc(bt_Node *root, int idx);
void bt_merge(bt_Node *root, int idx);
void bt_fillNode(bt_Node *root, int idx);
void bt_borrowFromPrev(bt_Node *root, int idx);
void bt_borrowFromNext(bt_Node *root, int idx);
void *bt_search(bt_Tree *tree, void *value);
void *bt_searchAux(bt_Tree *tree, bt_Node *root, void *value);
int bt_getHeight(bt_Node *root);

#endif // BT_H
