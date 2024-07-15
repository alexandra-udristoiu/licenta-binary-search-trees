#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

const bool BLACK = true;
const bool RED = false;


struct Node {
    int key;
    bool color;
    Node *left, *right, *parent;

    Node(int key, bool color, Node* parent, Node *left, Node *right) {
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->key = key;
        this->color = color;
    }
};

Node *insertFixLeft(Node *pNode);

Node *insertFixRight(Node *pNode);

void deleteFix(Node *pNode);

Node *deleteFixLeft(Node *pNode);

Node *deleteFixRight(Node *pNode);

Node *nil = new Node(0, BLACK, nullptr, nullptr, nullptr);
Node *root = nil;

void assignLeftChild(Node* node, Node *child) {
    if (node != nil) {
        node->left = child;
    }
    if (child != nil) {
        child->parent = node;
    }
}

void assignRightChild(Node* node, Node *child) {
    if (node != nil) {
        node->right = child;
    }
    if (child != nil) {
        child->parent = node;
    }
}

void rotateLeft(Node* node) {
    Node* rightNode = node->right;
    assignRightChild(node, rightNode->left);

    rightNode->parent = node->parent;
    if (node->parent != nil) {
        if (node == node->parent->left) {
            node->parent->left = rightNode;
        }
        else {
            node->parent->right = rightNode;
        }
    }

    assignLeftChild(rightNode, node);
    if (root == node) {
        root = rightNode;
    }
}

void rotateRight(Node* node) {
    Node *leftNode = node->left;
    assignLeftChild(node, leftNode->right);

    leftNode->parent = node->parent;
    if (node->parent != nil) {
        if (node == node->parent->left) {
            node->parent->left = leftNode;
        }
        else {
            node->parent->right = leftNode;
        }
    }

    assignRightChild(leftNode, node);
    if (root == node) {
        root = leftNode;
    }
}

Node *searchNode(Node* node, int x) {
    if (node->key == x || node == nil) {
        return node;
    }
    if (node->key > x) {
        return searchNode(node->left, x);
    }
    else {
        return searchNode(node->right, x);
    }
}

bool search(int x) {
    Node* node = searchNode(root, x);
    if (node != nil) {
        return true;
    }
    else {
        return false;
    }
}

void insertFix(Node *node) {
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            node = insertFixLeft(node);
        }
        else {
            node = insertFixRight(node);
        }
    }
    root->color = BLACK;
}

Node *insertFixLeft(Node* node) {
    Node* grandparent = node->parent->parent;
    Node* uncle = grandparent->right;
    if (uncle->color == RED) {
        node->parent->color = uncle->color = BLACK;
        grandparent->color = RED;
        node = grandparent;
    }
    else {
        if (node == node->parent->right) {
            rotateLeft(node->parent);
            node = node->left;
        }
        node->parent->color = BLACK;
        grandparent->color = RED;
        rotateRight(grandparent);
    }
    return node;
}

Node *insertFixRight(Node* node) {
    Node* grandparent = node->parent->parent;
    Node* uncle = grandparent->left;
    if (uncle->color == RED) {
        node->parent->color = uncle->color = BLACK;
        grandparent->color = RED;
        node = grandparent;
    }
    else {
        if (node == node->parent->left) {
            rotateRight(node->parent);
            node = node->right;
        }
        node->parent->color = BLACK;
        grandparent->color = RED;
        rotateLeft(grandparent);
    }
    return node;
}

void insert(int x) {
    if (root == nil) {
        root = new Node(x, BLACK, nil, nil, nil);
        return;
    }
    Node *node = root, *newNode = nullptr;
    while (newNode == nullptr) {
        if (node->key > x) {
            if (node->left == nil) {
                node->left = newNode = new Node(x, RED, node, nil, nil);
            }
            node = node->left;
        }
        else {
            if (node->right == nil) {
                node->right = newNode = new Node(x, RED, node, nil, nil);
            }
            node = node->right;
        }
    }
    insertFix(newNode);
}

void transplant(Node* node, Node* newNode) {
    newNode->parent = node->parent;
    if (node->parent == nil) {
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
    while (node->right != nil) {
        node = node->right;
    }
    return node;
}

void erase(int x) {
    Node *node = searchNode(root, x);
    Node *deletedNode, *movedNode;
    if (node->left == nil) {
        transplant(node, node->right);
        deletedNode = node;
        movedNode = node->right;
    }
    else {
        if (node->right == nil) {
            transplant(node, node->left);
            deletedNode = node;
            movedNode = node->left;
        }
        else {
            deletedNode = goRight(node->left);
            movedNode = deletedNode->left;
            node->key = deletedNode->key;
            transplant(deletedNode, deletedNode->left);
        }
    }
    if (deletedNode->color == BLACK) {
        deleteFix(movedNode);
    }
    delete deletedNode;
}

void deleteFix(Node *node) {
    while (node->color == BLACK && node != root) {
        if (node == node->parent->left) {
            node = deleteFixLeft(node);
        }
        else {
            node = deleteFixRight(node);
        }
    }
    node->color = BLACK;
}

Node *deleteFixLeft(Node *node) {
    Node* sibling = node->parent->right;
    if (sibling->color == RED) {
        swap(sibling->color, node->parent->color);
        rotateLeft(node->parent);
        sibling = node->parent->right;
    }
    if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
        sibling->color = RED;
        node = node->parent;
    }
    else {
        if (sibling->right->color == BLACK) {
            sibling->color = RED;
            rotateRight(sibling);
            sibling = node->parent->right;
            sibling->color = BLACK;
        }
        sibling->color = node->parent->color;
        node->parent->color = sibling->right->color = BLACK;
        rotateLeft(node->parent);
        node = root;
    }
    return node;
}

Node *deleteFixRight(Node *node) {
    Node* sibling = node->parent->left;
    if (sibling->color == RED) {
        swap(sibling->color, node->parent->color);
        rotateRight(node->parent);
        sibling = node->parent->left;
    }
    if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
        sibling->color = RED;
        node = node->parent;
    }
    else {
        if (sibling->left->color == BLACK) {
            sibling->color = RED;
            rotateLeft(sibling);
            sibling = node->parent->left;
            sibling->color = BLACK;
        }
        sibling->color = node->parent->color;
        node->parent->color = sibling->left->color = BLACK;
        rotateRight(node->parent);
        node = root;
    }
    return node;
}

void inorder(ostream& os, Node* node, int x, int y) {
    if (node != nil) {
        inorder(os, node->left, x, y);
        if (x <= node->key && node->key <= y) {
            os<< node->key <<" ";
        }
        inorder(os, node->right, x, y);
    }
}

int successor(Node* node, int x) {
    if (node == nil) {
        return x - 1;
    }
    if (node->key >= x) {
        int current = successor(node->left, x);
        if (current == x - 1) {
            current = node->key;
        }
        return current;
    }
    else {
        return successor(node->right, x);
    }
}

int predecessor(Node* node, int x) {
    if (node == nil) {
        return x + 1;
    }
    if (node->key <= x) {
        int current = predecessor(node->right, x);
        if (current == x + 1) {
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
