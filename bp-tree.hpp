#include <iostream>

#include "bp-tree.h"

using namespace std;

template<class K>
BpTree<K>::Node::Node(int size) {
    this->leaf = true;
    this->children = vector<Node*>(size, nullptr);
    this->key.reserve(size-1);
    this->value.reserve(size-1);
    this->parent = nullptr;
    this->nextleaf = nullptr;
}

template<class K>
BpTree<K>::BpTree(int node_len) {
    this->node_len = node_len;
    this->root = new BpTree::Node(this->node_len);
}

template<class K>
void BpTree<K>::insert(K key) {
    BpTree::Node* p = this->root;
    p = findPos(key);
    if (p != nullptr) insertInto(p, key);
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
typename BpTree<K>::Node* BpTree<K>::findPos(K key) {
    BpTree::Node* p = this->root;

    while (!p->leaf) {
        for (int i=0; i<p->key.size(); i++) {
            if (key <= p->key[i]) {
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
    // verifica se a chave já existe na árvore
    for (int i=0; i<p->key.size(); i++) {
        if (key == p->key[i]) {
            return nullptr;
        }
    }
    return p; // se p é uma leaf
}

template<class K>
int BpTree<K>::insertInto(Node* p, K key) {
    // se o nó ainda aceitar entradas
    if (p->key.size() < p->key.capacity()) {
        // se o nó for vazio...
        if (p->key.size() == 0 
        // ...ou se a chave for o mairo valor do nó
        || key > p->key[p->key.size()-1]) {
            p->key.push_back(key); // insere no final.
            return p->key.size();
        } else {
            // caso contrário, insere no meio do nó.
            for (int i = 0; i<=p->key.size(); i++) {
                if (key < p->key[i]) {
                    p->key.insert(p->key.begin()+i, key);
                    return i;
                }
            }
        }
    } else { // se o nó ficar sobrecarregado
        // cria um novo nó com tamanho 1 maior e inserta nele
        Node* u = new Node(this->node_len+1);
        u->key = p->key;
        u->children = p->children;
        insertInto(u, key);
        // cria dois novos vetores e o pai
        Node* q1 = new Node(this->node_len);
        q1->leaf = p->leaf;
        Node* q2 = new Node(this->node_len);
        q2->leaf = p->leaf;
        // define as nextleaves
        q1->nextleaf = q2;
        q2->nextleaf = p->nextleaf;
        // e divide os valores de u entre eles
        int division = 0;
        if (u->key.size()%2 == 0) division = u->key.size()/2;
        else division = (u->key.size()+1)/2;
        for (int i=0; i<u->key.size(); i++) {
            if (i < division-1) {
                q1->key.push_back(u->key[i]);
                q1->children.insert(q1->children.begin(), u->children[i]);
            } else if (i == division-1) {
                if (q1->leaf) {
                    q1->children.insert(q1->children.begin(), u->children[i]);
                }
                q1->children.push_back(u->children[i]);
                // se não for uma raiz
                if (p->parent != nullptr) {
                    int pos = insertInto(p->parent, u->key[i]);
                    vector<Node*> par_children = p->parent->children;
                    par_children.erase(par_children.begin()+pos-1);
                    par_children.insert(par_children.begin()+pos-1, q2);
                    par_children.insert(par_children.begin()+pos-1, q1);
                    // remove os nós children excedentes
                    while (par_children.size()>this->node_len) {
                        par_children.pop_back();
                    }
                    p->parent->children = par_children;
                    q1->parent = p->parent;
                    q2->parent = p->parent;
                } else { // se for uma raiz
                    Node* newroot = new Node(this->node_len);
                    newroot->leaf = false;
                    newroot->key.push_back(u->key[i]);
                    newroot->children.insert(newroot->children.begin(), q2);
                    newroot->children.insert(newroot->children.begin(), q1);
                    newroot->children.pop_back();
                    newroot->children.pop_back();
                    q1->parent = newroot;
                    q2->parent = newroot;
                    this->root = newroot;
                }
            } else {
                q2->key.push_back(u->key[i]);
                q2->children.insert(q2->children.begin(), u->children[i]);
            }
        }
        p = q1;
        delete u;
    }
    return 0;
}