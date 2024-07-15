#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cassert>
using namespace std;


struct Node {
    int key, level;
    Node *left, *right;

    Node(int key, int level, Node* left, Node* right) {
        this->key = key;
        this->level = level;
        this->left = left;
        this->right = right;
    }
};

Node *nil = new Node(-1, 0, nullptr, nullptr);

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

Node* skew(Node* node) {
    if (node != nil && node->left->level == node->level) {
        return rotateRight(node);
    }
    else {
        return node;
    }
}

Node* split(Node* node) {
    if (node->right != nil && node->right->right->level == node->level) {
        node = rotateLeft(node);
        node->level++;
    }
    return node;
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
    if (node == nil) {
        return new Node(x, 1, nil, nil);
    }
    else {
        if (node->key > x) {
            node->left = insert(node->left, x);
        }
        else {
            node->right = insert(node->right, x);
        }
        node = skew(node);
        node = split(node);
        return node;
    }
}

Node* goRight(Node* node) {
    while(node->right != nil) {
        node = node->right;
    }
    return node;
}

Node* erase(Node* node, int x) {
    if (node->key > x) {
        node->left = erase(node->left, x);
    }
    else {
        if (node->key < x) {
            node->right = erase(node->right, x);
        }
        else {
            if (node->left == nil && node->right == nil) {
                delete node;
                return nil;
            }
            if (node->left == nil) {
                node->key = node->right->key;
                node->right = erase(node->right, node->right->key);
            }
            else {
                Node* p = goRight(node->left);
                node->key = p->key;
                node->left = erase(node->left, p->key);;
            }
        }
    }
    node->level = 1 + min(node->right->level, node->left->level);
    node->right->level = min(node->level, node->right->level);
    node = skew(node);
    node->right = skew(node->right);
    if (node->right != nil) {
        node->right->right = skew(node->right->right);
    }
    node = split(node);
    if (node->right != nil) {
        node->right = split(node->right);
    }
    return node;
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

void check(Node* node) {
    if (node != nil) {
        check(node->left);
        check(node->right);
        assert(node->left->level + 1 == node->level);
        assert(node->right->level == node->level || node->right->level + 1 == node->level);
        if (node->right != nil) {
            assert(node->level > node->right->right->level);
        }
    }
}


int main() {
    ifstream fin("abce.in");
    ofstream fout("abce.out");

    int q;
    fin>> q;

    Node* root = nil;

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
        check(root);
    }
    return 0;
}