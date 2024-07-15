#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cassert>
using namespace std;

mt19937 randomGenerator(time(0));


struct Node {
    int key, priority;
    Node *left, *right, *parent;

    Node(int key) {
        this->left = this->right = this->parent = nullptr;
        this->key = key;
        this->priority = randomGenerator();
    }
};


Node* rotateLeft(Node* node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    rightChild->left = node;
    return rightChild;
}

Node* rotateRight(Node* node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;
    leftChild->right = node;
    return leftChild;
}

Node* search(Node* node, int x) {
    if (node == NULL) {
        return nullptr;
    }
    if (node->key == x) {
        return node;
    }
    if (node->key > x) {
        return search(node->left, x);
    }
    else {
        return search(node->right, x);
    }
}

Node* fingerSearch(Node* x, int k) {
    Node *v = x->parent, *currentNode = x, *z = nullptr;
    while (v != nullptr) {
        if (x->key < v->key) {
            if (k >= v->key) {
                currentNode = v;
            }
            else {
                break;
            }
        }
        if (currentNode != nullptr) {
            if (currentNode->key == k) {
                return currentNode;
            }
            if (currentNode->key < k) {
                currentNode = currentNode->right;
            } else {
                currentNode = currentNode->left;
            }
        }
        v = v->parent;
    }
    while (currentNode->key != k) {
        if (currentNode->key < k) {
            currentNode = currentNode->right;
        }
        else {
            currentNode = currentNode->left;
        }
    }
    return currentNode;
}

Node* insert(Node* node, int x) {
    if (node == NULL) {
        return new Node(x);
    }
    else {
        if (node->key > x) {
            node->left = insert(node->left, x);
            if (node->left->priority < node->priority) {
                node = rotateRight(node);
            }
        }
        else {
            node->right = insert(node->right, x);
            if (node->right->priority < node->priority) {
                node = rotateLeft(node);
            }
        }
        return node;
    }
}

Node* erase(Node* node, int x) {
    if (node->key == x) {
        if (node->left == NULL && node->right == NULL) {
            delete node;
            return NULL;
        }
        if (node->right == NULL || (node->left != NULL && node->right != NULL && node->left->priority < node->right->priority)) {
            node = rotateRight(node);
            node->right = erase(node->right, x);
        }
        else {
            node = rotateLeft(node);
            node->left = erase(node->left, x);
        }
        return node;
    }
    else {
        if (node->key > x) {
            node->left = erase(node->left, x);
        }
        else {
            node->right = erase(node->right, x);
        }
        return node;
    }
}

void inorder(ostream& os, Node* node, int x, int y) {
    if (node != NULL) {
        inorder(os, node->left, x, y);
        if (x <= node->key && node->key <= y) {
            os<< node->key <<" ";
        }
        inorder(os, node->right, x, y);
    }
}

int successor(Node* node, int x) {
    if (node == NULL) {
        return -1;
    }
    if (node->key >= x) {
        int current = successor(node->left, x);
        if (current == -1) {
            current = node->key;
        }
        return current;
    }
    else {
        return successor(node->right, x);
    }
}

int predecessor(Node* node, int x) {
    if (node == NULL) {
        return -1;
    }
    if (node->key <= x) {
        int current = predecessor(node->right, x);
        if (current == -1) {
            current = node->key;
        }
        return current;
    }
    else {
        return predecessor(node->left, x);
    }
}

void makePointers(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node->left != nullptr)
        node->left->parent = node;
    if (node->right != nullptr)
        node->right->parent = node;
    makePointers(node->left);
    makePointers(node->right);
}

int main() {
    ifstream fin("abce.in");
    ofstream fout("abce.out");

    int q;
    fin>> q;

    Node* root = NULL;

    for (int i = 1; i <= 10; i++) {
        root = insert(root, i);
    }
    makePointers(root);

    Node* y = fingerSearch(search(root,4), 10);
    cout<< y->key;

    return 0;
}