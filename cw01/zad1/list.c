#include "list.h"

void printList(List *list){
    Node *n = list->head;
    for (int i=0;i<list->size;i++){
        printPerson(n->person);
        printf("\n");
        n = n->next;
    }
}

List *initList(void) {
    List *new_list = (List *) malloc(sizeof(List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

void removeList(List *list){
    while (list->head != list->tail){
        Node *n = list->head;
        list = removeNode(list,n);
    }
    list = removeNode(list,list->head);
    free(list);
}

List *pushFront(List *list, Person *p) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->person = p;
    n->next = n->prev = NULL;
    if (list->size == 0)
        list->head = list->tail = n;
    else {
        n->next = list->head;
        list->head->prev = n;
        list->head = n;
    }
    list->size += 1;
    return list;
}

List *pushBack(List *list, Person *p) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->person = p;
    n->next = n->prev = NULL;
    if (list->size == 0)
        pushFront(list, p);
    else {
        n->prev = list->tail;
        list->tail->next = n;
        list->tail = n;
        list->size += 1;
    }
    return list;
}

Node *findNode(List *list, char *mail) {
    Node *n;
    n = list->head;
    while (strcmp(mail,n->person->email) != 0)
        n = n->next;
    return n;
}

List *insertAfter(List *list, Node *node, Person *p) {
    if (list->size == 0)
        return pushFront(list, p);
    Node *n = (Node *) malloc(sizeof(Node));
    n->person = p;
    n->next = node->next;
    node->next->prev = n;
    node->next = n;
    n->prev = node;
    list->size += 1;
    return list;
}

List *removeNode(List *list, Node *node) {
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else if (node == list->head) {
        list->head = node->next;
        node->next->prev = list->head;
        node->next = node->prev = NULL;

    } else if (node == list->tail) {
        list->tail = node->prev;
        node->prev->next = list->tail;
        node->next = node->prev = NULL;

    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = node->prev = NULL;
    }
    free(node);
    list->size -= 1;
    return list;
}

List *mergeList(List *list1, List *list2, char *key) {
    if (list1->size == 0)
        return list2;
    if (list2->size == 0)
        return list1;

    List *result = initList();
    Node *i1 = list1->head;
    Node *i2 = list2->head;
    int c1, c2;
    c1 = c2 = 0;
    int s1, s2;
    s1 = list1->size;
    s2 = list2->size;

    while (c1 < s1 && c2 < s2) {
        if (compareBy(i1->person,i2->person,key) <= 0) {
            result = pushBack(result, i1->person);
            i1 = i1->next;
            c1++;
        } else {
            result = pushBack(result, i2->person);
            i2 = i2->next;
            c2++;
        }
    }
    while (c1 < s1) {
        result = pushBack(result, i1->person);
        i1 = i1->next;
        c1++;
    }
    while (c2 < s2) {
        result = pushBack(result, i2->person);
        i2 = i2->next;
        c2++;
    }
    free(i1); free(i2);
    removeList(list1);
    removeList(list2);
    return result;
}

List *mergeThreeList(List *l1, List *l2, List *l3, char *key){
    List *result = mergeList(l1,l2,key);
    result = mergeList(result,l3,key);
    return result;
}

List *qSort(List *list, char *key){
    if (list->size == 1 || list->size == 0)
        return list;
    List *lss = initList();
    List *eq = initList();
    List *grt = initList();
    Person *pivot = list->tail->person;
    Node *n = list->head;
    for (int i=0; i<list->size;i++){
        if (compareBy(n->person,pivot,key) < 0) //n.person.key < pivot.key
            lss = pushBack(lss,n->person);
        else if (compareBy(n->person,pivot,key) == 0)
            eq = pushBack(eq,n->person);
        else
            grt = pushBack(grt,n->person);
        n = n->next;
    }
    return mergeThreeList(qSort(lss,key),eq,qSort(grt,key),key);
    //TODO : zwalnianie lss,eq,grt ?
}