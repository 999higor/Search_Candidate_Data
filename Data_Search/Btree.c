#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "BTree.h"

const int AVG_KEY_INDEX = MAX_KEYS >> 1;
const int MIN_DEGREE = (MAX_KEYS + 1) >> 1;
int tabs = -1;

void bt_valueArrayOpenGap(void *arr[], int s, int index)
{
	memmove(arr+index+1, arr+index, (s-index-1) * sizeof(void *));
}

void bt_valueArrayCloseGap(void *arr[], int s, int index)
{
	memmove(arr+index, arr+index+1, (s-index-1) * sizeof(void *));
}

int bt_valueArrayInsert(void *arr[], int s, int n, void *value, bt_compareCallback cmp)
{
	int i;
	for(i = 0; i < n; i++) {
		if(cmp(arr[i], value) == 0)
			return 0;
		if(cmp(arr[i], value) > 0) {
			bt_valueArrayOpenGap(arr, s, i);
			break;
		}
	}

	arr[i] = value;
	return 1;
}

void bt_nodeArrayOpenGap(bt_Node *arr[], int s, int index)
{
	memmove(arr+index+1, arr+index, (s-index-1) * sizeof(bt_Node *));
}

void bt_nodeArrayCloseGap(bt_Node *arr[], int s, int index)
{
	memmove(arr+index, arr+index+1, (s-index-1) * sizeof(bt_Node *));
}

void bt_nodeArrayPrint(bt_Node *arr[], int n)
{
	int i;
	for(i = 0; i < n; i++)
		printf("%p ", arr[i]);
	printf("\n");
}

bt_Tree *bt_newTree(bt_compareCallback cmpCb, bt_freeCallback freeCb)
{
	bt_Tree *t = (bt_Tree *)malloc(sizeof(bt_Tree));
	t->root = NULL;
	t->cmpCb = cmpCb;
	t->freeCb = freeCb;
	// t->typeSize = typeSize;
	return t;
}

bt_Node *bt_newNode(int isLeaf)
{
	bt_Node *temp = (bt_Node *)malloc(sizeof(bt_Node));
	temp->isLeaf = isLeaf;
	temp->numKeys = 0;
	int i;
	for(i = 0; i < MAX_KEYS+1; i++)
		temp->children[i] = NULL;

	return temp;
}

void bt_free(bt_Tree *tree)
{
	bt_freeAux(tree, tree->root);
	free(tree);
}

void bt_freeAux(bt_Tree *tree, bt_Node *root)
{
	if(root != NULL) {
		int i;
		for(i = 0; i < root->numKeys; i++) {
			bt_freeAux(tree, root->children[i]);
			tree->freeCb(root->keys[i]);
		}
		bt_freeAux(tree, root->children[i]);

		free(root);
	}
}

void bt_insert(bt_Tree *tree, void *value)
{
	if(tree->root == NULL) {
		tree->root = bt_newNode(1);
		tree->root->keys[0] = value;
		tree->root->numKeys++;
	} else {
		bt_insertNonRoot(tree, tree->root, value);

		if(tree->root->numKeys == MAX_KEYS) {
			bt_Node *newRoot = bt_newNode(0);
			newRoot->children[0] = tree->root;
			bt_splitChild(tree, newRoot, 0);
			tree->root = newRoot;
		}
	}
}

void bt_insertNonRoot(bt_Tree *tree, bt_Node *root, void *value)
{
	int i;

	if(root->isLeaf) {
		if(bt_valueArrayInsert(root->keys, MAX_KEYS, root->numKeys, value, tree->cmpCb) == 1)
			root->numKeys++;
	}
	else {
		for(i = 0; i < root->numKeys && tree->cmpCb(root->keys[i], value) < 0; i++);
		if(i < root->numKeys && tree->cmpCb(root->keys[i], value) == 0)
			return;

		bt_insertNonRoot(tree, root->children[i], value);

		if(root->children[i]->numKeys == MAX_KEYS)
			bt_splitChild(tree, root, i);
	}
}

void bt_splitChild(bt_Tree *tree, bt_Node *root, int index)
{
	bt_Node *l = root->children[index];
	bt_Node *r = bt_newNode(l->isLeaf);
	int firstI = AVG_KEY_INDEX+1;

	memmove(r->keys, &l->keys[firstI], (MAX_KEYS-firstI) * sizeof(void *));
	memmove(r->children, &l->children[firstI], (MAX_KEYS-firstI+1) * sizeof(bt_Node *));
	r->numKeys = l->numKeys - firstI;
	l->numKeys = AVG_KEY_INDEX;

	bt_valueArrayOpenGap(root->keys, MAX_KEYS, index);
	root->keys[index] = l->keys[AVG_KEY_INDEX];

	bt_nodeArrayOpenGap(root->children, MAX_KEYS+1, index+1);
	root->children[index+1] = r;
	root->numKeys++;
}

void bt_fprintNode(bt_Node *root, FILE *fp, bt_toStrCallback toStr)
{
	int j;
	if(root != NULL) {
		tabs++;
		int i;
		for(i = 0; i < root->numKeys; i++) {
			if(root->isLeaf == 0) bt_fprintNode(root->children[i], fp, toStr);
			for(j = 0; j < tabs; j++) fprintf(fp, "\t");
			fprintf(fp, "%s\n", toStr(root->keys[i]));
		}
		if(root->isLeaf == 0) bt_fprintNode(root->children[root->numKeys], fp, toStr);
		tabs--;
	}
	else {
		for(j = 0; j < tabs; j++) fprintf(fp, "\t");
		fprintf(fp, "<empty>\n");
	}
}

void bt_remove(bt_Tree *tree, void *value)
{
	if(tree->root != NULL) {
		bt_removeNonRoot(tree, tree->root, value);

		if(tree->root->numKeys == 0) {
			bt_Node *temp = tree->root;
			if(tree->root->isLeaf)
				tree->root = NULL;
			else
				tree->root = tree->root->children[0];

			free(temp);
		}
	}
}

void bt_removeNonRoot(bt_Tree *tree, bt_Node * root, void *value)
{
	int idx;
	for(idx = 0; idx < root->numKeys && tree->cmpCb(root->keys[idx], value) < 0; idx++);

	if(idx < root->numKeys && tree->cmpCb(root->keys[idx], value) == 0) {
		if(root->isLeaf) {
			bt_valueArrayCloseGap(root->keys, MAX_KEYS, idx);
			root->numKeys--;
		} else {
			bt_removeFromNonLeaf(tree, root, idx);
		}
	} else if(!root->isLeaf) {
		int flag = (idx == root->numKeys);

		if(root->children[idx]->numKeys < MIN_DEGREE)
			bt_fillNode(root, idx);

		if(flag && idx > root->numKeys)
			bt_removeNonRoot(tree, root->children[idx-1], value);
		else
			bt_removeNonRoot(tree, root->children[idx], value);
	}
}

void bt_removeFromNonLeaf(bt_Tree *tree, bt_Node *root, int idx) {
	if(root->children[idx]->numKeys >= MIN_DEGREE) {
		void *pred = bt_getPred(root, idx);
		root->keys[idx] = pred;
		bt_removeNonRoot(tree, root->children[idx], pred);
	} else if(root->children[idx+1]->numKeys >= MIN_DEGREE) {
		void *succ = bt_getSucc(root, idx);
		root->keys[idx] = succ;
		bt_removeNonRoot(tree, root->children[idx+1], succ);
	} else {
		void *value = root->keys[idx];
		bt_merge(root, idx);
		bt_removeNonRoot(tree, root->children[idx], value);
	}
}

void *bt_getPred(bt_Node *root, int idx)
{
	bt_Node *cur = root->children[idx];
	while(!cur->isLeaf)
		cur = cur->children[cur->numKeys];

	return cur->keys[cur->numKeys-1];
}

void *bt_getSucc(bt_Node *root, int idx)
{
	bt_Node *cur = root->children[idx+1];
	while(!cur->isLeaf)
		cur = cur->children[0];

	return cur->keys[0];
}

void bt_merge(bt_Node *root, int idx)
{
	bt_Node *child = root->children[idx];
	bt_Node *sibling = root->children[idx+1];

	child->keys[MIN_DEGREE-1] = root->keys[idx];
	//memmove(child->keys+MIN_DEGREE, sibling->keys, sibling->numKeys*sizeof(int));
	int i;
	for(i = 0; i < sibling->numKeys; i++) {
		child->keys[i+MIN_DEGREE] = sibling->keys[i];
	}
	if(!child->isLeaf) {
		//memmove(child->children+MIN_DEGREE, sibling->children, (sibling->numKeys+1)*sizeof(bt_Node *));
		for(i = 0; i < sibling->numKeys+1; i++) {
			child->children[i+MIN_DEGREE] = sibling->children[i];
		}
	}
	bt_valueArrayCloseGap(root->keys, MAX_KEYS, idx);
	bt_nodeArrayCloseGap(root->children, MAX_KEYS+1, idx+1);
	child->numKeys += sibling->numKeys + 1;
	root->numKeys--;
	free(sibling);
}

void bt_fillNode(bt_Node *root, int idx)
{
	if(idx > 0 && root->children[idx-1]->numKeys >= MIN_DEGREE)
		bt_borrowFromPrev(root, idx);
	else if(idx < root->numKeys && root->children[idx+1]->numKeys >= MIN_DEGREE)
		bt_borrowFromNext(root, idx);
	else {
		if(idx < root->numKeys)
			bt_merge(root, idx);
		else
			bt_merge(root, idx-1);
	}
}

void bt_borrowFromPrev(bt_Node *root, int idx)
{
	bt_Node *child = root->children[idx];
	bt_Node *sibling = root->children[idx-1];

	bt_valueArrayOpenGap(child->keys, MAX_KEYS, 0);
	if(!child->isLeaf)
		bt_nodeArrayOpenGap(child->children, MAX_KEYS+1, 0);

	child->keys[0] = root->keys[idx-1];
	if(!child->isLeaf)
		child->children[0] = sibling->children[sibling->numKeys];

	root->keys[idx-1] = sibling->keys[sibling->numKeys-1];
	sibling->numKeys--;
	child->numKeys++;
}


void bt_borrowFromNext(bt_Node *root, int idx)
{
	bt_Node *child = root->children[idx];
	bt_Node *sibling = root->children[idx+1];

	child->keys[child->numKeys] = root->keys[idx];
	if(!child->isLeaf)
		child->children[child->numKeys+1] = sibling->children[0];

	root->keys[idx] = sibling->keys[0];
	bt_valueArrayCloseGap(sibling->keys, MAX_KEYS, 0);
	if(!sibling->isLeaf)
		bt_nodeArrayCloseGap(sibling->children, MAX_KEYS+1, 0);

	sibling->numKeys--;
	child->numKeys++;
}

void *bt_search(bt_Tree *tree, void *value)
{
	return bt_searchAux(tree, tree->root, value);
}

void *bt_searchAux(bt_Tree *tree, bt_Node *root, void *value)
{
	int i = 0;
	while(i < root->numKeys && tree->cmpCb(root->keys[i], value) < 0)
		i++;

	if(i < root->numKeys && tree->cmpCb(root->keys[i], value) == 0)
		return root->keys[i];

	if(root->isLeaf == 0)
		return bt_searchAux(tree, root->children[i], value);

	return NULL;
}

int bt_getHeight(bt_Node *root)
{
	if(root == NULL)
		return -1;

	int i = 0;
	while(root->isLeaf == 0) {
		i++;
		root = root->children[0];
	}

	return i;
}
