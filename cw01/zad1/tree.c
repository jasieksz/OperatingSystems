#include "tree.h"


TNode *initTree(char *key) {
    TNode *n = (TNode *) malloc(sizeof(TNode));
    n->parent = n->left = n->right = NULL;
    n->person = NULL;
    n->key = key;
    return n;
}

TNode *insertPerson(TNode *root, Person *p){
    if (root == NULL) {
        root = initTree("emai");
        root->person = p;
        return root;
    } else {
        TNode *temp;
        if (compareBy(p,root->person,root->key)<=0)
        {
            temp = insertPerson(root->left, p);
            root->left  = temp;
            temp->parent = root;
        }
        else
        {
            temp = insertPerson(root->right,p);
            root->right = temp;
            temp->parent = root;
        }
        return root;
    }
}

TNode *findElement(TNode *root, Person *person){
    if (root == NULL || personEqual(root->person,person) == 0)
        return root;
    if (compareBy(person,root->person,root->key) <= 0) //x < root->val
        return findElement(root->left,person);
    else //if (compareBy(person,root->person,root->key) > 0) //x > root.val
        return findElement(root->right,person);
}

TNode *minElement(TNode *root){
    TNode *tmp = root;
    while (tmp->left != NULL)
        tmp = tmp->left;
    return tmp;
}

TNode *inOrderSuccessor(TNode *elem){
    if (elem->right != NULL)
        return minElement(elem->right);
    TNode *tmp = elem->parent;
    while (tmp != NULL && elem == tmp->right){
        elem = tmp;
        tmp = tmp->parent;
    }
    return tmp;
}

TNode *removeElement(TNode *root, TNode *elem){
    if (root == NULL)
        return root;
    if (compareBy(elem->person,root->person,root->key) < 0) //elem < root
        root->left = removeElement(root->left,elem);
    else if (compareBy(elem->person,root->person,root->key) > 0) //(elem->val > root->val)
        root->right = removeElement(root->right,elem);
    else {
        if (root->left == NULL){
            TNode *tmp =root->right;
            free(root);
            return tmp;
        } else if (root->right == NULL){
            TNode *tmp = root->left;
            free(root);
            return tmp;
        }
        TNode *tmp = minElement(root->right); //succesor
        root->person = tmp->person;
        root->right = removeElement(root->right,tmp);
        free(tmp);
    }
    return root;
}

void inOrderTraversal(TNode *root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printPerson(root->person);
        inOrderTraversal(root->right);
    }
}

TNode *rebuildTree(TNode *root, char *key){
    TNode *tree = initTree(key);
    TNode *n = inOrderSuccessor(root);
    while (n != NULL){
        //TNode *newElement = initTree(n->person,key);
        tree = insertPerson(tree,n->person);
        n = inOrderSuccessor(n);
    }
    return tree;
}

void deleteTree(TNode *node) {
    if (node == NULL) return;
    else if (node->left == NULL && node->right == NULL) {
        destroyPerson(node->person);
        free(node);
    } else {
        deleteTree(node->left);
        deleteTree(node->right);
        destroyPerson(node->person);
        free(node);
    }
}
