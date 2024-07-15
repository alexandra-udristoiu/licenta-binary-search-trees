#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

struct Node {
    int key, rank;
    Node *left, *right, *parent;

    Node(int key, int rank, Node* parent, Node *left, Node *right) {
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->key = key;
        this->rank = rank;
    }

    int getLeftDifference() {
        return this->rank - left->rank;
    }

    int getRightDifference() {
        return this->rank - right->rank;
    }
};

Node *nil = new Node(0, -1, nullptr, nullptr, nullptr);
Node *root = nil;

void assignLeftChild(Node* node, Node *child) {
    node->left = child;
    child->parent = node;
}

void assignRightChild(Node* node, Node *child) {
    node->right = child;
    child->parent = node;
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
    swap(rightNode->rank, node->rank);

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
    swap(leftNode->rank, node->rank);

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

Node* getSibling(Node *node) {
    if (node == node->parent->left) {
        return node->parent->right;
    }
    else {
        return node->parent->left;
    }
}

void insertFix(Node* node) {
    node = node->parent;
    node->rank++;
    while (node != root && node->parent->rank - node->rank == 0) {
        Node* sibling = getSibling(node);
        Node* parent = node->parent;
        if (parent->rank - sibling->rank == 1) {
            parent->rank++;
            node = node->parent;
        }
        else {
            if (node == parent->left) {
                if (node->getRightDifference() == 2) {
                    rotateRight(parent);
                }
                else {
                    rotateLeft(node);
                    rotateRight(parent);
                }
            }
            else {
                if (node->getLeftDifference() == 2) {
                    rotateLeft(parent);
                }
                else {
                    rotateRight(node);
                    rotateLeft(parent);
                }
            }
            parent->rank--;
            break;
        }
    }
}

void insert(int x) {
    if (root == nil) {
        root = new Node(x, 0, nil, nil, nil);
        return;
    }
    Node *node = root, *newNode = nullptr;
    while (newNode == nullptr) {
        if (node->key > x) {
            if (node->left == nil) {
                node->left = newNode = new Node(x, 0, node, nil, nil);
            }
            node = node->left;
        }
        else {
            if (node->right == nil) {
                node->right = newNode = new Node(x, 0, node, nil, nil);
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

void deleteFix(Node* node) {
    Node* parent = node->parent;
    if (parent->left == nil && parent->right == nil && parent->rank == 1) {
        parent->rank = 0;
        node = parent;
    }

    while (node->parent->rank - node->rank == 3) {
        parent = node->parent;
        Node* sibling = getSibling(node);
        if (parent->rank - sibling->rank == 2) {
            parent->rank--;
            node = node->parent;
            continue;
        }
        if (sibling->getLeftDifference() == 2 && sibling->getRightDifference() == 2) {
            parent->rank--;
            sibling->rank--;
            node = node->parent;
            continue;
        }
        if (node == parent->left) {
            if (sibling->getRightDifference() == 1) {
                rotateLeft(parent);
                if (parent->left == nil && parent->right == nil) {
                    parent->rank--;
                }
            }
            else {
                rotateRight(sibling);
                rotateLeft(parent);
                parent->rank--;
            }
        }
        else {
            if (sibling->getLeftDifference() == 1) {
                rotateRight(parent);
                if (parent->left == nil && parent->right == nil) {
                    parent->rank--;
                }
            }
            else {
                rotateLeft(sibling);
                rotateRight(parent);
                parent->rank--;
            }
        }
        break;
    }
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
    delete deletedNode;
    deleteFix(movedNode);
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

void check(Node* node) {
    if (node != nil) {
        if (node->left == nil && node->right == nil) {
            assert(node->rank == 0);
        }
        else {
            assert(node->getLeftDifference() >= 1 && node->getLeftDifference() <= 2);
            assert(node->getRightDifference() >= 1 && node->getRightDifference() <= 2);
            check(node->left);
            check(node->right);
        }
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
        //check(root);
    }

    return 0;
}

