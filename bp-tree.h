#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include <vector>

using namespace std;

template<class K, class V>
class BpTree {
    struct Node {
        // Node básico padrão
        bool leaf;
        vector<K> key;
        vector<V> value;
        vector<Node*> children;
        Node* parent;
        Node* nextleaf;

        Node(int);
    };

public:
    Node* root = nullptr;
    int node_len = 5;

    // Constructor
    explicit BpTree(int node_len);

    V find(K x);

    vector<V> findRange(K k1, K k2);

    void insert(K x, V v);

    void remove(K x);

    void print();

    private:
    // Encontra o nó de insert
    Node* searchNode(K x);

    // Insere a chave no nó p
    void insertIntoLeaf(Node* p, K x, V v);

    // Insere a chave no nó pai p
    void insertIntoParent(Node* p, Node* q, K x);

    // Remove a chave no nó p
    void removeKey(Node* p, K x);

    // Retorna posição de divisão do nó no split
    int getDivision();

    // Retorna o valor mínimo de chaves que determina se um nó será reestruturado
    int getThreshold(bool leaf);
};

#include "bp-tree.hpp"
#endif
