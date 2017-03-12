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
}Person;

typedef struct Node {
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    Person *person;
    char *key;
} Node;


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

Node *initTree(Person *person, char *key);
Node *insertElement(Node *root, Node *elem);
Node *findElement(Node *root, Person *p);
Node *minElement(Node *root);
Node *inOrderSuccessor(Node *elem);
Node *removeElement(Node *root, Node *elem);
void inOrderTraversal(Node *root);
Node *rebuildTree(Node *root, char *key);



void printPerson(Person *person) {
    if (person != NULL) {
        printf("first_name: %s \n", person->first_name);
        //printf("last_name: %s \n", person->last_name);
       // printf("birth_date: %s \n", person->birth_date);
        //printf("email: %s \n", person->email);
        printf("phone_number: %s \n", person->phone_number);
       // printf("address: %s \n", person->address);
    }
    else {
        printf("No such person \n");
    }
    printf("\n");
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

Node *initTree(Person *person, char *key) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->parent = n->left = n->right = NULL;
    n->person = person;
    n->key = key;
    return n;
}

Node *insertElement(Node *root, Node *elem) {
    if (root == NULL)
        return elem;
    else {
        Node *tmp;
        if (compareBy(elem->person,root->person,root->key) <= 0) { //(elem->val <= root->val)
            tmp = insertElement(root->left, elem);
            root->left = tmp;
            tmp->parent = root;
            tmp->key = root->key;

        } else{
            tmp = insertElement(root->right, elem);
            root->right = tmp;
            tmp->parent = root;
            tmp->key = root->key;
        }
        //TODO : free tmp
        return root;
    }
}

Node *findElement(Node *root, Person *person){
    if (root == NULL || personEqual(root->person,person) == 0)
        return root;
    if (compareBy(person,root->person,root->key) < 0) //x < root->val
        return findElement(root->left,person);
    else if (compareBy(person,root->person,root->key) > 0) //x > root.val
        return findElement(root->right,person);
}

Node *minElement(Node *root){
    Node *tmp = root;
    while (tmp->left != NULL)
        tmp = tmp->left;
    return tmp;
}

Node *inOrderSuccessor(Node *elem){
    if (elem->right != NULL)
        return minElement(elem->right);
    Node *tmp = elem->parent;
    while (tmp != NULL && elem == tmp->right){
        elem = tmp;
        tmp = tmp->parent;
    }
    return tmp;
}

Node *removeElement(Node *root, Node *elem){
    if (root == NULL)
        return root;
    if (compareBy(elem->person,root->person,root->key) < 0) //elem < root
        root->left = removeElement(root->left,elem);
    else if (compareBy(elem->person,root->person,root->key) > 0) //(elem->val > root->val)
        root->right = removeElement(root->right,elem);
    else {
        if (root->left == NULL){
            Node *tmp =root->right;
            free(root);
            return tmp;
        } else if (root->right == NULL){
            Node *tmp = root->left;
            free(root);
            return tmp;
        }
        Node *tmp = minElement(root->right); //succesor
        root->person = tmp->person;
        root->right = removeElement(root->right,tmp);
        //TODO : free(tmp)
    }
    return root;
}

void inOrderTraversal(Node *root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printPerson(root->person);
        inOrderTraversal(root->right);
    }
}

Node *rebuildTree(Node *root, char *key){
    Node *tree = initTree(root->person, key);
    Node *n = inOrderSuccessor(root);
    while (n != NULL){
        Node *newElement = initTree(n->person,key);
        tree = insertElement(tree,newElement);
        n = inOrderSuccessor(n);
    }
    return tree;
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
    Person *p1 = makePerson("j","Sznajd","27.12.1996","j@g.pl","53","zaw");
    Person *p2 = makePerson("a","Kowalski","13.12.2001","a@g.pl","55","ale");
    Person *p3 = makePerson("z","Faza","07.05.1241","z@g.pl","57","pol");
    Node *tree = initTree(p1,"first");
    Node *e2 = initTree(p2,tree->key);
    Node *e3 = initTree(p3,tree->key);
    tree = insertElement(tree,e2);
    tree = insertElement(tree,e3);
    inOrderTraversal(tree);
    Node *newTree = rebuildTree(minElement(tree),"phone");
    printf("\n");
    inOrderTraversal(newTree);

    return 0;
}