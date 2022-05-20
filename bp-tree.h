#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include <vector>

using namespace std;

template<class K>
class BpTree {
    struct Node {
        // Node básico padrão
        bool leaf;
        int nChildren;
        vector<K> key;
        vector<Node*> children;
        vector<K> value;

        Node(int);
    };

public:
    Node* root = nullptr;
    int node_len = 5;

    // Constructor
    explicit BpTree(int node_len);

    bool find(K x);

    // Encontra a posição de insert
    Node* findPos(K x);

    void insert(K x);

    void remove(K x);

    void print();
};

#include "bp-tree.hpp"
#endif
