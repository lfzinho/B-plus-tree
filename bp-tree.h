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

    // Retorna o valor associado a chave key
    V find(K x);

    // retorna o valor dos nós no intervalo [k1, k2]
    vector<V> findRange(K k1, K k2);

    // Inserta uma nova chave
    void insert(K x, V v);

    // Remove uma chave
    void remove(K x);

    // Limpa a árvore
    void clear();

    // Imprime a árvore
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

    // Limpa a árvore com função recursiva
    void clear(Node* p);

    // Retorna posição de divisão do nó no split
    int getDivision();

    // Retorna o valor mínimo de chaves que determina se um nó será reestruturado
    int getThreshold(bool leaf);
};

#include "bp-tree.hpp"
#endif
