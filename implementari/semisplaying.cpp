#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;


struct Node {
    int key;
    Node *left, *right, *parent;

    Node(int key, Node* parent) {
        this->left = this->right = nullptr;
        this->parent = parent;
        this->key = key;
    }
};

Node *root = nullptr;

void assignLeftChild(Node* node, Node *child) {
    if (node != nullptr) {
        node->left = child;
    }
    if (child != nullptr) {
        child->parent = node;
    }
}

void assignRightChild(Node* node, Node *child) {
    if (node != nullptr) {
        node->right = child;
    }
    if (child != nullptr) {
        child->parent = node;
    }
}

void rotateLeft(Node* node) {
    Node* rightNode = node->right;
    assignRightChild(node, rightNode->left);

    rightNode->parent = node->parent;
    if (node->parent != nullptr) {
        if (node == node->parent->left) {
            node->parent->left = rightNode;
        }
        else {
            node->parent->right = rightNode;
        }
    }

    assignLeftChild(rightNode, node);
}

void rotateRight(Node* node) {
    Node *leftNode = node->left;
    assignLeftChild(node, leftNode->right);

    leftNode->parent = node->parent;
    if (node->parent != nullptr) {
        if (node == node->parent->left) {
            node->parent->left = leftNode;
        }
        else {
            node->parent->right = leftNode;
        }
    }

    assignRightChild(leftNode, node);
}

Node* splay(Node *node) {
    while (node->parent != nullptr) {
        Node *parent = node->parent;
        if (parent->parent == nullptr) {
            node = parent;
        }
        else {
            Node* grandParent = parent->parent;
            if (parent == grandParent->left && node == parent->left) {
                rotateRight(grandParent);
                node = parent;
            }
            if (parent == grandParent->right && node == parent->right) {
                rotateLeft(grandParent);
                node = parent;
            }
            if (parent == grandParent->left && node == parent->right) {
                rotateLeft(parent);
                rotateRight(grandParent);
            }
            if (parent == grandParent->right && node == parent->left) {
                rotateRight(parent);
                rotateLeft(grandParent);
            }
        }
    }
    return node;
}

Node *searchNode(int x) {
    Node *node = root, *previous = nullptr;
    while (node != nullptr && node->key != x) {
        previous = node;
        if (node->key > x) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    if (node == nullptr) {
        return previous;
    }
    return node;
}

bool search(int x) {
    if (root == nullptr) {
        return false;
    }
    Node *node = searchNode(x);
    root = splay(node);
    if (node->key == x) {
        return true;
    }
    else {
        return false;
    }
}

void insert(int x) {
    if (root == nullptr) {
        root = new Node(x, nullptr);
        return;
    }
    Node *node = root, *newNode = nullptr;
    while (newNode == nullptr) {
        if (node->key > x) {
            if (node->left == nullptr) {
                node->left = newNode = new Node(x, node);
            }
            node = node->left;
        }
        else {
            if (node->right == nullptr) {
                node->right = newNode = new Node(x, node);
            }
            node = node->right;
        }
    }
    root = splay(newNode);
}

void transplant(Node* node, Node* newNode) {
    if (newNode != nullptr) {
        newNode->parent = node->parent;
    }
    if (node->parent == nullptr) {
        root = newNode;
    }
    else {
        if (node == node->parent->left) {
            node->parent->left = newNode;
        }
        else {
            node->parent->right = newNode;
        }
    }
}

Node* goRight(Node *node) {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

void erase(int x) {
    Node *node = searchNode(x);
    Node *deletedNode;
    if (node->left == nullptr) {
        transplant(node, node->right);
        deletedNode = node;
    }
    else {
        if (node->right == nullptr) {
            transplant(node, node->left);
            deletedNode = node;
        }
        else {
            deletedNode = goRight(node->left);
            node->key = deletedNode->key;
            transplant(deletedNode, deletedNode->left);
        }
    }
    if (node->parent != nullptr) {
        root = splay(node->parent);
    }
    delete deletedNode;
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

Node* successor(Node* node, int x) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->key >= x) {
        Node* current = successor(node->left, x);
        if (current == nullptr) {
            current = node;
        }
        return current;
    }
    else {
        return successor(node->right, x);
    }
}

int successor(int x) {
    Node *node = successor(root, x);
    root = splay(node);
    return node->key;
}

Node* predecessor(Node* node, int x) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->key <= x) {
        Node* current = predecessor(node->right, x);
        if (current == nullptr) {
            current = node;
        }
        return current;
    }
    else {
        return predecessor(node->left, x);
    }
}

int predecessor(int x) {
    Node *node = predecessor(root, x);
    root = splay(node);
    return node->key;
}

int main() {
    ifstream fin("abce.in");
    ofstream fout("abce.out");

    int q;
    fin>> q;

    for (; q; q--) {
        int type, x, y;
        fin>> type;

        switch (type) {
            case 1:
                fin>> x;
                insert(x);
                break;
            case 2:
                fin>> x;
                erase(x);
                break;
            case 3:
                fin>> x;
                fout<< search(x) <<"\n";
                break;
            case 4:
                fin>> x;
                fout<< predecessor(x) <<"\n";
                break;
            case 5:
                fin>> x;
                fout<< successor(x) <<"\n";
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