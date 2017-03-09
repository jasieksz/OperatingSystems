#include <stdio.h>
#include <malloc.h>
#include <string.h>


typedef struct Person {
    char *first_name;
    char *last_name;
    char *birth_date;
    char *email;
    char *phone_number;
    char *address;
} Person;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    Person person;
    int value;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    int size;
} List;

List *initList(void);

List *pushFront(List *list, int x);

List *pushBack(List *list, int x);

List *insertAfter(List *list, Node *node, int x);

Node *findNode(List *list, int x);

List *removeNode(List *list, Node *node);

List *mergeList(List *list1, List *list2);

List *mergeThreeList(List *l1, List *l2, List *l3);

void printList(List *list);

void printPerson(Person person);

List *makeList(int z);

List *initList(void) {
    List *new_list = (List *) malloc(sizeof(List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

List *pushFront(List *list, int x) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->value = x;
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

List *pushBack(List *list, int x) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->value = x;
    n->next = n->prev = NULL;
    if (list->size == 0)
        pushFront(list, x);
    else {
        n->prev = list->tail;
        list->tail->next = n;
        list->tail = n;
        list->size += 1;
    }

    return list;
}

List *insertAfter(List *list, Node *node, int x) {
    if (list->size == 0)
        return pushFront(list, x);
    Node *n = (Node *) malloc(sizeof(Node));
    n->value = x;
    n->next = node->next;
    node->next->prev = n;
    node->next = n;
    n->prev = node;
    list->size += 1;
    return list;
}

Node *findNode(List *list, int x) {
    Node *n;
    n = list->head;
    while (n->value != x)
        n = n->next;
    return n;
}

List *removeNode(List *list, Node *node) {
    if (list->size == 1) {
        list->head = list->tail = NULL; //TODO : czy free(node) usuwa też jego wskaźniki czy trzeba je dac na null?
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

List *mergeList(List *list1, List *list2) {
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
        if (i1->value <= i2->value) {
            result = pushBack(result, i1->value);
            i1 = i1->next;
            c1++;
        } else {
            result = pushBack(result, i2->value);
            i2 = i2->next;
            c2++;
        }
    }
    while (c1 < s1) {
        result = pushBack(result, i1->value);
        i1 = i1->next;
        c1++;
    }
    while (c2 < s2) {
        result = pushBack(result, i2->value);
        i2 = i2->next;
        c2++;
    }
    return result;
}

void printList(List *list) {
    Node *n;
    n = list->head;
    for (int i = 0; i < list->size; i++) {
        printPerson(n->person);
        n = n->next;
    }
    printf("\n");
}

List *makeList(int z) {
    List *list = initList();
    while (z--) {
        int a;
        scanf("%i", &a);
        list = pushBack(list, a);
    }
    return list;
}

List *qSort(List *list, int comparator(Person,Person)){
    if (list->size == 1 || list->size == 0)
        return list;
    List *lss = initList();
    List *eq = initList();
    List *grt = initList();
    Person pivot = list->tail->person;
    Node *n = list->head;
    for (int i=0; i<list->size;i++){
        if (comparator(n->person,pivot) < 0)
            lss = pushBack(lss,n->value);
        else if (comparator(n->person,pivot) == 0)
            eq = pushBack(eq,n->value);
        else
            grt = pushBack(grt,n->value);
        n = n->next;
    }
    return mergeThreeList(qSort(lss,comparator),eq,qSort(grt,comparator));
}

int comparatorName(Person p1, Person p2){
    return strcmp(p1.first_name,p2.first_name);
}



List *mergeThreeList(List *l1, List *l2, List *l3){
    List *result = mergeList(l1,l2);
    result = mergeList(result,l3);
    return result;
}

void printPerson(Person person) {
        printf("first_name: %s \n", person.first_name);
        printf("last_name: %s \n", person.last_name);
        printf("day_of_birth: %s \n", person.birth_date);
        printf("email: %s \n", person.email);
        printf("phone_number: %s \n", person.phone_number);
        printf("address: %s \n", person.address);
}


int main() {
    List *lista1 = initList();
    printList(lista1);
    printf("\n");
    lista1 = qSort(lista1,comparatorName);
    printList(lista1);

    return 0;
}
/*
    Person person;
    person.first_name = "Jan";
    person.last_name = "Sobieski";
    lista1 = pushFront(lista1,person);

    Person secondPerson;
    secondPerson.first_name = "Stefan";
    secondPerson.last_name = "Batory";
    lista1 = pushFront(lista1,secondPerson);


    Person thirdPerson;
    thirdPerson.first_name = "Boleslaw";
    thirdPerson.last_name = "Chrobry";
    lista1 = pushFront(lista1, thirdPerson);

    Person fourthPerson;
    fourthPerson.first_name = "Henryk";
    fourthPerson.last_name = "Walezy";
    lista1 = pushFront(lista1, fourthPerson);
*/
