#include <iostream>

#include "bp-tree.h"

using namespace std;

template<class K>
BpTree<K>::Node::Node(int size) {
    this->leaf = true;
    this->nChildren = 0;
    this->children = vector<Node*>(size, nullptr);
    this->key.reserve(size-1);
    this->value.reserve(size-1);
}

template<class K>
BpTree<K>::BpTree(int node_len) {
    this->node_len = node_len;
    this->root = new BpTree::Node(this->node_len);
}

template<class K>
void BpTree<K>::insert(K key) {
    BpTree::Node* p = this->root;
    p = findPos(p, key);

    p->key.push_back(key);
    p->value.push_back(key);
    p->nChildren ++;
}

template<class K>
bool BpTree<K>::find(K key) {
    BpTree::Node* p = this->root;

    while (p != nullptr) {
        for (int i=0; i<p->key.size(); i++) {
            if (key < p->key[i]) {
                // se a key for menor q a atual
                p = p->children[i];
                break;
            } else if (i+1 == p->key.size() && key > p->key[i]) {
                // se for a ultima key, e ela for maior
                p = p->children[i+1];
                break;
            } else if (key == p->key[i]) {
                // encontrou a chave
                return true;
            }
        }
    }
    return false; // se p é um nullptr
}

template<class K>
BpTree<K>::Node* BpTree<K>::findPos(K key) {
    BpTree::Node* p = this->root;

    while (!p->leaf) {
        for (int i=0; i<p->key.size(); i++) {
            if (key < p->key[i]) {
                // se a key for menor q a atual
                p = p->children[i];
                break;
            } else if (i+1 == p->key.size() && key > p->key[i]) {
                // se for a ultima key, e ela for maior
                p = p->children[i+1];
                break;
            }
        }
    }
    return p; // se p é uma leaf
}
