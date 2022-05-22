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
        vector<K> key;
        vector<Node*> children;
        vector<K> value;
        Node* parent;
        Node* nextleaf;

        Node(int);
    };

public:
    Node* root = nullptr;
    int node_len = 5;

    // Constructor
    explicit BpTree(int node_len);

    bool find(K x);

    void insert(K x);

    void remove(K x);

    void print();

    private:
    // Encontra a posição de insert
    Node* findPos(K x);

    // Insere a chave no nó p
    int insertInto(Node* p, K x);
};

#include "bp-tree.hpp"
#endif
