#ifndef ZAD1_LIST_H
#define ZAD1_LIST_H

#include "person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    Person *person;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    int size;
}List;

List *initList (void);
List *pushFront (List *list, Person *person);
List *pushBack (List *list, Person *person);
Node *findNode (List *list, char *mail);
List *insertAfter (List *list, Node *node, Person *person);
List *removeNode (List *list, Node *node);
List *mergeList (List *list1, List *list2, char *key);
List *mergeThreeList(List *l1, List *l2, List *l3, char *key);
List *qSort(List *list, char *key);
void printList (List *list);
void removeList(List *list);
#endif //ZAD1_LIST_H
