#ifndef ZAD1_TREE_H
#define ZAD1_TREE_H


#include "person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct TNode {
    struct TNode *parent;
    struct TNode *left;
    struct TNode *right;
    Person *person;
    char *key;
} TNode;

TNode *initTree(char *key);
TNode *insertPerson(TNode *root, Person *p);
//TNode *insertElement(TNode *root, TNode *elem);
TNode *findElement(TNode *root, Person *p);
TNode *minElement(TNode *root);
TNode *inOrderSuccessor(TNode *elem);
TNode *removeElement(TNode *root, TNode *elem);
void inOrderTraversal(TNode *root);
TNode *rebuildTree(TNode *root, char *key);
void deleteTree(TNode *root);

#endif //ZAD1_TREE_H
