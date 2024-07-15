#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

mt19937 randomGenerator(time(0));


struct Node {
    int key, priority;
    Node *left, *right;

    Node(int key) {
        this->left = this->right = nullptr;
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
    if (node == nullptr) {
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

Node* join(Node* u, Node* v) {
    if (u == nullptr) {
        return v;
    }
    if (v == nullptr) {
        return u;
    }
    if (u->priority < v->priority) {
        u->right = join(u->right, v);
        return u;
    }
    else {
        v->left = join(u, v->left);
        return v;
    }
}

pair<Node*, Node*> unzip(Node* node, int x) {
    if (node == nullptr) {
        return make_pair(nullptr, nullptr);
    }
    if (node->key > x) {
        pair<Node*, Node*> trees = split(node->left, x);
        node->left = trees.second;
        return make_pair(trees.first, node);
    }
    else {
        pair<Node*, Node*> trees = split(node->right, x);
        node->right = trees.first;
        return make_pair(node, trees.second);
    }
}

Node* insert(Node* root, int x) {
    Node *node = new Node(x);
    pair<Node*, Node*> trees = split(root, x);
    trees.first = join(trees.first, node);
    return join(trees.first, trees.second);
}

Node* erase(Node* root, int x) {
    pair<Node *, Node *> trees = split(root, x);
    trees.first = split(trees.first, x - 1).first;
    return join(trees.first, trees.second);
}

void inorder(ostream& os, Node* node, int x, int y) {
    if (node != nullptr) {
        inorder(os, node->left, x, y);
        if (x <= node->key && node->key <= y) {
            os<< node->key <<" ";
        }
        inorder(os, node->right, x, y);
    }
}

int successor(Node* node, int x) {
    if (node == nullptr) {
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
    if (node == nullptr) {
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

    Node* root = nullptr;

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
