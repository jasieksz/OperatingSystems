#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct Person {
    char *first_name;
    char *last_name;
    char *birth_date;
    char *email;
    char *phone_number;
    char *address;
}Person;

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

void printPerson(Person *person);
Person *makePerson(char *fst_n, char *lst_n, char *b_d, char *email, char *phone, char *adrs);
int personEqual(Person *p1, Person *p2);
int compareBy(Person *p1, Person *p2, char *key);
int firstNameComparator(Person *person1, Person *person2);
int lastNameComparator(Person *person1, Person *person2);
int birthDateComparator(Person *person1, Person *person2);
int emailComparator(Person *person1, Person *person2);
int phoneComparator(Person *person1, Person *person2);
int addressComparator(Person *person1, Person *person2);


List *initList (void);
List *pushFront (List *list, Person *person);
List *pushBack (List *list, Person *person);
Node *findNode (List *list, Person *person);
List *insertAfter (List *list, Node *node, Person *person);
List *removeNode (List *list, Node *node);
List *mergeList (List *list1, List *list2, char *key);
List *mergeThreeList(List *l1, List *l2, List *l3, char *key);
List *qSort(List *list, char *key);
void printList (List *list);
void printList(List *list);



void printList(List *list){
    Node *n = list->head;
    for (int i=0;i<list->size;i++){
        printPerson(n->person);
        printf("\n");
        n = n->next;
    }
}

void printPerson(Person *person) {
    if (person != NULL) {
        printf("first_name: %s \n", person->first_name);
        printf("last_name: %s \n", person->last_name);
        printf("birth_date: %s \n", person->birth_date);
        printf("email: %s \n", person->email);
        printf("phone_number: %s \n", person->phone_number);
        printf("address: %s \n", person->address);
    }
    else {
        printf("No such person \n");
    }
}
Person *makePerson(char *fst_n, char *lst_n, char *b_d, char *email, char *phone, char *adrs){
    Person *p = (Person *) malloc(sizeof(Person));
    p->first_name = fst_n;
    p->last_name = lst_n;
    p->birth_date = b_d;
    p->email = email;
    p->phone_number = phone;
    p->address = adrs;
    return p;
}

List *initList(void) {
    List *new_list = (List *) malloc(sizeof(List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
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

Node *findNode(List *list, Person *p) {
    Node *n;
    n = list->head;
    while (personEqual(n->person,p) != 0)
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
    //TODO : czy musze zwolnic i1,i2?
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
/*
 * PERSON COMPARATORS
 */
int personEqual(Person *p1, Person *p2){ //0 -> rowne, -1 -> nie istnieje/nie rowne
    if (p1 == NULL || p2 == NULL) return -1;
    if (p1->first_name == p2->first_name
        && p1->last_name == p2->last_name
        && p1->birth_date == p2->birth_date
        && p1->email == p2->email
        && p1->phone_number == p2->phone_number
        && p1->address == p2->address) return 0;
    else
        return -1;
}


int compareBy(Person *p1, Person *p2, char *key){
    if (strcmp(key,"first")==0) return firstNameComparator(p1,p2);
    else if (strcmp(key,"last")==0) return lastNameComparator(p1,p2);
    else if (strcmp(key,"birth")==0) return birthDateComparator(p1,p2);
    else if (strcmp(key,"email")==0) return emailComparator(p1,p2);
    else if (strcmp(key,"phone")==0) return phoneComparator(p1,p2);
    else if (strcmp(key,"address")==0) return addressComparator(p1,p2);
}

int firstNameComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->first_name != NULL
        && person2->first_name != NULL) {
        return strcmp(person1->first_name, person2->first_name);
    }
    return 0;
}

int lastNameComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->last_name != NULL
        && person2->last_name != NULL) {
        return strcmp(person1->last_name, person2->last_name);
    }
    return 0;
}

int birthDateComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->birth_date != NULL
        && person2->birth_date != NULL) {
        return strcmp(person1->birth_date, person2->birth_date);
    }
    return 0;
}

int emailComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->email != NULL
        && person2->email != NULL) {
        return strcmp(person1->email, person2->email);
    }
    return 0;
}

int phoneComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->phone_number != NULL
        && person2->phone_number != NULL) {
        return strcmp(person1->phone_number, person2->phone_number);
    }
    return 0;
}

int addressComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->address != NULL
        && person2->address != NULL) {
        return strcmp(person1->address, person2->address);
    }
    return 0;
}



int main(){

    List *lista1 = initList();
    Person *p1 = makePerson("j","Sznajd","27.12.1996","j@g.pl","53","zaw");
    Person *p2 = makePerson("a","Kowalski","13.12.2001","a@g.pl","4","ale");
    Person *p3 = makePerson("z","Faza","07.05.1241","z@g.pl","7","pol");
    lista1 = pushBack(lista1,p1);
    lista1 = pushBack(lista1,p2);
    lista1 = pushBack(lista1,p3);
    lista1 = qSort(lista1,"phone");
    printList(lista1);
    int a;
    scanf("%d",&a);

    return 0;
}
