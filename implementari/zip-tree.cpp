#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

mt19937 randomGenerator(time(0));


int getPriority() {
    int p = 1;
    while (randomGenerator() % 2 == 1) {
        p++;
    }
    return p;
}

struct Node {
    int key, priority;
    Node *left, *right;

    Node(int key) {
        this->left = this->right = nullptr;
        this->key = key;
        this->priority = getPriority();
    }
};

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

Node* join(Node* nodStanga, Node* nodDreapta) {
    if (nodStanga == nullptr) {
        return nodDreapta;
    }
    if (nodDreapta == nullptr) {
        return nodStanga;
    }
    if (nodStanga->priority < nodDreapta->priority) {
        nodStanga->right = zip(nodStanga->right, nodDreapta);
        return nodStanga;
    }
    else {
        nodDreapta->left = zip(nodStanga, nodDreapta->left);
        return nodDreapta;
    }
}

pair<Node*, Node*> unzip(Node* node, int x) {
    if (node == nullptr) {
        return make_pair(nullptr, nullptr);
    }
    if (node->key > x) {
        pair<Node*, Node*> trees = unzip(node->left, x);
        node->left = trees.second;
        return make_pair(trees.first, node);
    }
    else {
        pair<Node*, Node*> trees = unzip(node->right, x);
        node->right = trees.first;
        return make_pair(node, trees.second);
    }
}

Node* insert(Node* node, Node *v) {
    if (node == nullptr) {
        return v;
    }
    if (node->priority < v->priority || (node->priority == v->priority && node->key > v->key)) {
        pair<Node*, Node*> children = unzip(node, v->key);
        v->left = children.first;
        v->right = children.second;
        return v;
    }
    if (node->key > v->key) {
        node->left = insert(node->left, v);
    }
    else {
        node->right = insert(node->right, v);
    }

    return node;
}

Node* erase(Node* node, int x) {
    if (node->key == x) {
        return zip(node->left, node->right);
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
                root = insert(root, new Node(x));
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


