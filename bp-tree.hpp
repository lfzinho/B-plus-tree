#include <iostream>

#include "bp-tree.h"

using namespace std;

template<class K, class V>
BpTree<K, V>::Node::Node(int size) {
    leaf = true;
    children.reserve(size+1);
    key.reserve(size);
    value.reserve(size);
    parent = nullptr;
    nextleaf = nullptr;
}

template<class K, class V>
BpTree<K, V>::BpTree(int node_len) {
    node_len = node_len;
    root = new BpTree::Node(node_len);
}

template<class K, class V>
bool BpTree<K, V>::find(K x) {
    Node* p = searchNode(x);
    // procura pela chave no nó ideal
    for (int i=0; i < p->key.size(); i++) {
        if (x == p->key[i]) {
            return true;
        }
    }
    return false;
}

template<class K, class V>
void BpTree<K, V>::insert(K x, V v) {
    Node* p = searchNode(x);
    insertIntoLeaf(p, x, v);

    // Se o nó der overflow:
    if (p->key.size() == node_len) {
        Node* q = new Node(node_len);
        q->parent = p->parent;
        int div = getDivision();

        // distribui os valores de p overflowed entre p e q
        K middle_value;
        vector<K> p_keys;
        vector<Node*> p_children;
        p_keys.reserve(node_len);
        p_children.reserve(node_len+1);
        for (int i=0; i<p->key.size(); i++) {
            if (i <= div) {
                p_keys.push_back(p->key[i]);
                p_children.push_back(p->children[i]);
            } else {
                q->key.push_back(p->key[i]);
                q->children.push_back(q->children[i]);
            }
        }
        p->key = p_keys;
        p->children = p_children;
        q->nextleaf = p->nextleaf;
        p->nextleaf = q;
        insertIntoParent(p, q, middle_value);
    }
}

template<class K, class V>
typename BpTree<K, V>::Node* BpTree<K, V>::searchNode(K x) {
    Node* p = root;
    while (!p->leaf) {
        for (int i=0; i<p->key.size(); i++) {
            if (x < p->key[i]) {
                // se o valor é menor que o nó atual
                p = p->children[i];
                break;
            } else if ((x == p->key[i])||(i+1 == p->key.size())){
                // se o valor é igual ao nó atual
                // ou se chegamos ao final do nó
                p = p->children[i+1];
                break;
            }
        }
    }
    return p;
}

template<class K, class V>
void BpTree<K, V>::insertIntoLeaf(Node* p, K x, V v) {
    if (p->key.size() > 0) {
        // se há chaves para iterar
        for (int i=0; i<p->key.size(); i++) {
            if (x <= p->key[i]) {
                // se a chave for <= que o valor atual
                auto pos_k = p->key.begin() + i;
                auto pos_v = p->value.begin() + i;
                p->key.insert(pos_k, x);
                p->value.insert(pos_v, x);
                break;

            } else if (i+1 == p->key.size()) {
                // se chegar ao final do nó
                p->key.push_back(x);
                p->value.push_back(v);
                break;
            }
        }
    } else {
        // se o nó está vazio
        p->key.push_back(x);
        p->value.push_back(v);
    }
}

template<class K, class V>
void BpTree<K, V>::insertIntoParent(Node* p, Node* q, K x) {
    if (p == root) {
        // Se a child original era a raiz
        Node* n_root = new Node(node_len);
        n_root->key.push_back(x);
        n_root->children.push_back(p);
        n_root->children.push_back(q);
        root = n_root;
        p->parent = n_root;
        q->parent = n_root;
    } else {
        // Se a child original era um nó comum
        Node* r = p->parent;
        for (int i; i<r->children.size(); i++) {
            if (r->children[i] == p) {
                auto pos_k = r->key.begin()+i;
                auto pos_c = r->children.begin()+i;
                r->key.insert(pos_k, x);
                r->children.insert(pos_c+1, q);
                if (r->key.size() == node_len) {
                    // Se dá overflow aqui, faz um novo split no r
                    Node* q2 = new Node(node_len);
                    q2->parent = r->parent;
                    int div = getDivision();
                    
                    // distribui os valores de r de acordo
                    K middle_value;
                    vector<K> r_keys;
                    vector<Node*> r_children;
                    r_keys.reserve(node_len);
                    r_keys.reserve(node_len+1);
                    for (int j=0; j<r->key.size(); j++) {
                        if (j < div) {
                            // valores abaixo da div
                                r_keys.push_back(r->key[j]);
                                r_children.push_back(r->children[j]);
                        } else if ( j == div) {
                            // valor na div
                            if (div == 0){
                                r_keys.push_back(r->key[j]);
                            }
                            r_children.push_back(r->children[j]);
                            middle_value = r->key[j];
                        } else {
                            // valores acima da div
                            q2->key.push_back(r->key[j]);
                            q2->children.push_back(r->children[j]);
                        }
                    }
                    r->key = r_keys;
                    r->children = r_children;

                    // corrige as filiações
                    for (int j=0; j<r->children.size(); j++) {
                        r->children[j]->parent = r;
                    }
                    for (int j=0; j<q2->children.size(); j++) {
                        q2->children[j]->parent = q2;
                    }

                    // sobe o nó do meio
                    insertIntoParent(r, q2, middle_value);
                }
                return;
            }
        }
    }
}

template<class K, class V>
int BpTree<K, V>::getDivision() {
    if (node_len%2 == 0) {
        return node_len/2-1;
    } else {
        return (node_len+1)/2-1;
    }
}

template<class K, class V>
void BpTree<K, V>::remove(K x, V v = NULL) {
    Node* p = searchNode(x);

    for (int i=0; i<p->key.size(); i++) {
        if (p->key[i] == x && (p->value = NULL || p->value[i] == v)) { // se encontrar a posição
            if (p == root) {
                // e estiver na raiz, apaga
                auto posk = p->key.begin() + i;
                auto posv = p->value.begin() + i;
                p->key.erase(posk);
                p->value.erase(posv);
            } else {
                // e estiver em outro nível, executa removeKey
                p->key.erase(posk);
                p->value.erase(posv);
                removeKey(p, x, v);          
            }
        }
    }
}

template<class K, class V>
void BpTree<K, V>::removeKey(Node* p, K x, V v) {}