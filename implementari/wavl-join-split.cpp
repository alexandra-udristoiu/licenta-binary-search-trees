#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

struct Node {
    int k, r;
    Node *left, *right, *parent;

    Node(int key, int rank, Node* parent, Node *left, Node *right) {
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->k = key;
        this->r = rank;
    }

    int getLeftDifference() {
        return this->r - left->r;
    }

    int getRightDifference() {
        return this->r - right->r;
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
    swap(rightNode->r, node->r);

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
    swap(leftNode->r, node->r);

    if (root == node) {
        root = leftNode;
    }
}

Node *searchNode(Node* node, int x) {
    if (node->k == x || node == nil) {
        return node;
    }
    if (node->k > x) {
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

Node* insertFix(Node* p) {
    if (max(p->left->r, p->right->r) < p->r) {
        return p;
    }
    Node* x = p->left, *s = p->right;
    if (x->r < s->r) {
        swap(x, s);
    }
    if (p->r - s->r == 1) {
        p->r++;
    } else {
        if (x == p->left) {
            if (x->r - x->right->r == 2) {
                rotateRight(p);
            } else {
                rotateLeft(x);
                rotateRight(p);
            }
        } else {
            if (x->r - x->left->r == 2) {
                rotateLeft(p);
            } else {
                rotateRight(x);
                rotateLeft(p);
            }
        }
        p->r--;
        p = p->parent;
    }
    return p;
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
    if (parent->left == nil && parent->right == nil && parent->r == 1) {
        parent->r = 0;
        node = parent;
    }

    while (node->parent->r - node->r == 3) {
        parent = node->parent;
        Node* sibling = getSibling(node);
        if (parent->r - sibling->r == 2) {
            parent->r--;
            node = node->parent;
            continue;
        }
        if (sibling->getLeftDifference() == 2 && sibling->getRightDifference() == 2) {
            parent->r--;
            sibling->r--;
            node = node->parent;
            continue;
        }
        if (node == parent->left) {
            if (sibling->getRightDifference() == 1) {
                rotateLeft(parent);
                if (parent->left == nil && parent->right == nil) {
                    parent->r--;
                }
            }
            else {
                rotateRight(sibling);
                rotateLeft(parent);
                parent->r--;
            }
        }
        else {
            if (sibling->getLeftDifference() == 1) {
                rotateRight(parent);
                if (parent->left == nil && parent->right == nil) {
                    parent->r--;
                }
            }
            else {
                rotateLeft(sibling);
                rotateRight(parent);
                parent->r--;
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
            node->k = deletedNode->k;
            transplant(deletedNode, deletedNode->left);
        }
    }
    delete deletedNode;
    deleteFix(movedNode);
}

Node* joinRight(int k, Node* node, Node* root2) {
    if (node->r <= root2->r + 1) {
        Node* x = new Node(k, node->r + 1, nil, node, root2);
        node->parent = root2->parent = x;
        return x;
    }
    else {
        node->right = joinRight(k, node->right, root2);
        node->right->parent = node;
        return insertFix(node);
    }
}

Node* joinLeft(int k, Node* root1, Node* node) {
    if (node->r <= root1->r + 1) {
        Node* x = new Node(k, node->r + 1, nil, root1, node);
        node->parent = root1->parent = x;
        return x;
    }
    else {
        node->left = joinLeft(k, root1, node->left);
        node->left->parent = node;
        return insertFix(node);
    }
}

Node* join(int k, Node* root1, Node* root2) {
    if (root1->r - root2->r > 1) {
        return joinRight(k, root1, root2);
    }
    if (root2->r - root1->r > 1) {
        return joinLeft(k, root1, root2);
    }
    Node* x =  new Node(k, max(root1->r, root2->r) + 1, nil, root1, root2);
    root1->parent = root2->parent = x;
    return x;
}

pair<Node*, Node*> split(Node* v, int x) {
    if (v == nil) {
        return make_pair(nil, nil);
    }
    if (v->k <= x) {
        pair<Node*, Node*> trees = split(v->right, x);
        trees.first = join(v->k, v->left, trees.first);
        return trees;
    }
    else {
        pair<Node*, Node*> trees = split(v->left, x);
        trees.second = join(v->k, trees.second, v->right);
        return trees;
    }
}

void insert(int x) {
    pair<Node*, Node*> trees = split(root, x);
    root = join(x, trees.first, trees.second);
}

void inorder(ostream& os, Node* node, int x, int y) {
    if (node != nil) {
        inorder(os, node->left, x, y);
        if (x <= node->k && node->k <= y) {
            os << node->k << " ";
        }
        inorder(os, node->right, x, y);
    }
}

int successor(Node* node, int x) {
    if (node == nil) {
        return x - 1;
    }
    if (node->k >= x) {
        int current = successor(node->left, x);
        if (current == x - 1) {
            current = node->k;
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
    if (node->k <= x) {
        int current = predecessor(node->right, x);
        if (current == x + 1) {
            current = node->k;
        }
        return current;
    }
    else {
        return predecessor(node->left, x);
    }
}

int nr, nr2;

void check(Node* node) {
    if (node != nil) {
        nr2++;
        if (node->left == nil && node->right == nil) {
            assert(node->r == 0);
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
                nr++;
                break;
            case 2:
                fin>> x;
                erase(x);
                nr--;
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
        cout<< type <<"\n";
        nr2 = 0;
        check(root);
        assert(nr == nr2);
    }

    return 0;
}


