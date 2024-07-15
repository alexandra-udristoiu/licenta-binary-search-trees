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
    Node *left, *right;

    Node(int key) {
        this->left = this->right = NULL;
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

bool search(Node* node, int x) {
    if (node == NULL) {
        return false;
    }
    if (node->key == x) {
        return true;
    }
    if (node->key > x) {
        return search(node->left, x);
    }
    else {
        return search(node->right, x);
    }
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

int main() {
    ifstream fin("abce.in");
    ofstream fout("abce.out");

    int q;
    fin>> q;

    Node* root = NULL;

    for (; q; q--) {
        int type, x, y;
        fin>> type;

        switch (type) {
            case 1:
                fin>> x;
                root = insert(root, x);
                break;
            case 2:
                fin>> x;
                root = erase(root, x);
                break;
            case 3:
                fin>> x;
                fout<< search(root, x) <<"\n";
                break;
            case 4:
                fin>> x;
                fout<< predecessor(root, x) <<"\n";
                break;
            case 5:
                fin>> x;
                fout<< successor(root, x) <<"\n";
                break;
            case 6:
                fin>> x >> y;
                inorder(fout, root, x, y);
                fout<<"\n";
                break;
        }
    }

    return 0;
}