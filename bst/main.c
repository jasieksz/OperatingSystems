#include <stdio.h>
#include <malloc.h>


typedef struct Node {
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    int val;
} Node;

Node *initTree(int x);
Node *insertElement(Node *root, Node *elem);
Node *findElement(Node *root, int x);
Node *minElement(Node *root);
Node *inOrderSuccessor(Node *elem);
Node *removeElement(Node *root, Node *elem);
void *printTree(Node *root);
Node *makeTree(int z);

Node *initTree(int x) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->parent = n->left = n->right = NULL;
    n->val = x;
    return n;
}

Node *insertElement(Node *root, Node *elem) {
    if (root == NULL)
        return elem;
    else {
        Node *tmp;
        if (elem->val <= root->val) {
            tmp = insertElement(root->left, elem);
            root->left = tmp;
            tmp->parent = root;

        } else{
            tmp = insertElement(root->right, elem);
            root->right = tmp;
            tmp->parent = root;
        }
        return root;
    }
}

Node *findElement(Node *root, int x){
    if (root == NULL || root->val == x)
        return root;
    if (x < root->val)
        return findElement(root->left,x);
    else if (x > root->val)
        return findElement(root->right,x);
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
    if (elem->val < root->val)
        root->left = removeElement(root->left,elem);
    else if (elem->val > root->val)
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
        root->val = tmp->val;
        root->right = removeElement(root->right,tmp);
    }
    return root;
}

void *printTree(Node *root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%d \n", root->val);
        printTree(root->right);
    }
    return NULL;
}

Node *makeTree(int z){
    Node *root = NULL;
    while (z--){
        int a;
        scanf("%d",&a);
        Node *elem = initTree(a);
        root = insertElement(root,elem);
    }
    return root;
}

int main() {

    Node *tree = makeTree(7);
    Node *e = findElement(tree,5);
    tree = removeElement(tree,e);
    printTree(tree);
}