#include <iostream>

#include "bp-tree.h"

using namespace std;

template<class K, class V>
BpTree<K, V>::Node::Node(int size) {
    leaf = false;
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
    root->leaf = true;
}

template<class K, class V>
V BpTree<K, V>::find(K x) {
    Node* p = searchNode(x);
    // procura pela chave no nó ideal
    for (int i=0; i < p->key.size(); i++) {
        if (x == p->key[i]) {
            return p->value[i];
        }
    }
    return NULL;
}

template<class K, class V>
vector<V> BpTree<K, V>::findRange(K x1, K x2) {
    vector<V> result;

    // procura pela menor chave
    Node* p = searchNode(x1);
    for (int i=0; i < p->key.size(); i++) {
        if (p->key[i] >= x1) {
            int j = i; // adiciona um novo iterador
            // começa a adicionar as respostas a um vetor
            while ((p->key[j] <= x2) && (p != nullptr)) {
                result.push_back(p->value[j]);
                j++;
                // se o elemento j sai dos limites do nó
                if (j > p->key.size()-1) {
                    // reseta j e avança p
                    j = 0;
                    p = p->nextleaf;
                }
            }
            break; // sai do loop depois que o while termina
        }
    }

    return result;
}

template<class K, class V>
void BpTree<K, V>::insert(K x, V v) {
    Node* p = searchNode(x);
    insertIntoLeaf(p, x, v);

    // Se o nó der overflow:
    if (p->key.size() == node_len) {
        Node* q = new Node(node_len);
        q->leaf = true;
        q->parent = p->parent;
        int div = getDivision();

        // distribui os valores de p overflowed entre p e q
        K middle_value;
        vector<K> p_keys;
        vector<V> p_values;
        vector<Node*> p_children;
        p_keys.reserve(node_len);
        p_children.reserve(node_len+1);
        for (int i=0; i<p->key.size(); i++) {
            if (i < div) {
                p_keys.push_back(p->key[i]);
                p_values.push_back(p->value[i]);
                p_children.push_back(p->children[i]);
            } else if (i == div) {
                p_keys.push_back(p->key[i]);
                p_values.push_back(p->value[i]);
                p_children.push_back(p->children[i]);
                middle_value = p->key[i];
            } else {
                q->key.push_back(p->key[i]);
                q->value.push_back(p->value[i]);
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
        for (int i=0; i<r->children.size(); i++) {
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
                    for (int j=0; j<r->children.size(); j++) {
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
                            if (j < r->key.size()) {
                                q2->key.push_back(r->key[j]);
                            }
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
int BpTree<K, V>::getThreshold(bool leaf) {
    if (leaf){
        if (node_len%2 == 0) {
            return (node_len)/2;
        } else {
            return (node_len-1)/2;
        }
    } else {
        if (node_len%2 == 0) {
            return (node_len)/2;
        } else {
            return (node_len+1)/2;
        }
    }
}

template<class K, class V>
void BpTree<K, V>::remove(K x) {
    Node* p = searchNode(x);

    for (int i=0; i<p->key.size(); i++) {
        if (p->key[i] == x) { // se encontrar a posição
            if (p == root) {
                // e estiver na raiz, apaga
                auto posk = p->key.begin() + i;
                auto posv = p->value.begin() + i;
                p->key.erase(posk);
                p->value.erase(posv);
            } else {
                // e estiver em outro nível, executa removeKey
                auto posk = p->key.begin() + i;
                auto posv = p->value.begin() + i;
                p->key.erase(posk);
                p->value.erase(posv);
                removeKey(p, x);          
            }
        }
    }
}

template<class K, class V>
void BpTree<K, V>::removeKey(Node* p, K x) {

    // se passarmos por um nó que não é folha
    if (!p->leaf){
        for (int i=0; i<p->key.size(); i++) {
            // remove o valor do nó
            if (p->key[i] = x) {
                auto posk = p->key.begin() + i;
                auto posv = p->value.begin() + i;
                p->key.erase(posk);
                p->value.erase(posv);
            }
        }
    }

    // se estivermos deletando a única chave na árvore
    if (p == root && p->children.size() == 1) {
        // transformamos a primeira children em root
        root = p->children[0];
        p->children[0]->parent = nullptr;
        // e deletamos a root
        delete root;
        return;
    }

    // se o nó não for raiz
    if (p != root) {
        if ( (!(p->leaf) && (p->children.size() < getThreshold(p->leaf))) // se o nó não for folha e condição1
        || ((p->leaf) && (p->children.size() < getThreshold(p->leaf))) // ou se o nó for folha e condição2
        ) {
            // Define variáveis auxiliares
            bool is_predecessor = false;
            Node* parentNode = p->parent;
            Node* PrevNode = nullptr;
            Node* NextNode = nullptr;
            Node* ndash = nullptr;
            K PrevK;
            K NextK;
            K value_;

            // localiza a posição do nó em relação ao pai
            for (int i=0; i<parentNode->children.size(); i++) {
                if (parentNode->children[i] == p) {
                    // i é a posição do nó
                    if (i > 0) {
                        PrevNode = parentNode->children[i-1];
                        PrevK = parentNode->key[i-1];
                    }
                    if ( i < parentNode->key.size() -1) {
                        NextNode = parentNode->children[i+1];
                        NextK = parentNode->value[i];
                    }
                }
            }

            if (PrevNode == nullptr) {
                ndash = NextNode;
                value_ = NextK;
            } else if (NextNode == nullptr) {
                is_predecessor = true;
                ndash = PrevNode;
                value_ = PrevK;
            } else {
                if (p->key.size() + NextNode->key.size() < node_len) {
                    ndash = NextNode;
                    value_ = NextK;
                } else {
                    is_predecessor = true;
                    ndash = PrevNode;
                    value_ = PrevK;
                }
            }
            
            if (p->key.size() + ndash->key.size() < node_len) {
                if (!is_predecessor) {
                    Node* placeholder = p;
                    p = ndash;
                    ndash = placeholder;
                }
                for (int i=0; i<p->children.size(); i++) {
                    ndash->children.push_back(p->children[i]);
                }
                if (!p->leaf) {
                    ndash->key.push_back(value_);
                } else {
                    ndash->nextleaf = p->nextleaf;
                }
                for (int i=0; i<p->key.size(); i++) {
                    ndash->key.push_back(p->key[i]);
                }

                if (!ndash->leaf) {
                    // corrige os parentescos
                    for (int i=0; i<ndash->children.size(); i++) {
                        ndash->children[i]->parent = ndash;
                    }
                }
                removeKey(p->parent, value_);
                delete p;

            } else { 
                if (is_predecessor) {
                    if (!p->leaf) {
                        Node* ndashpn = ndash->children.back();
                        ndash->children.pop_back();
                        K ndashkn = ndash->key.back();
                        ndash->key.pop_back();

                        p->children.insert(p->children.begin(), ndashpn);
                        p->key.insert(p->key.begin(), ndashkn);
                        parentNode = p->parent;
                        for (int i=0; i<parentNode->key.size(); i++) {
                            if (parentNode->key[i] == value_) {
                                parentNode->key[i] = ndashkn;
                                break;
                            }
                        }
                    } else {
                        Node* ndashpn = ndash->children.back();
                        ndash->children.pop_back();
                        K ndashkn = ndash->key.back();
                        ndash->key.pop_back();

                        p->children.insert(p->children.begin(), ndashpn);
                        p->key.insert(p->key.begin(), ndashkn);
                        parentNode = p->parent;
                        for (int i=0; i<p->key.size(); i++) {
                            if (parentNode->key[i] == value_) {
                                parentNode->key[i] = ndashkn;
                                break;
                            }
                        }
                    }
                } else {
                    if (!p->leaf) {
                        Node* ndashpn = ndash->children.front();
                        ndash->children.erase(ndash->children.begin());
                        K ndashkn = ndash->key.front();
                        ndash->key.erase(ndash->key.begin());

                        p->children.push_back(ndashpn);
                        p->key.push_back(ndashkn);
                        parentNode = p->parent;
                        for (int i=0; i<parentNode->key.size(); i++) {
                            if (parentNode->key[i] == value_) {
                                parentNode->key[i] = ndashkn;
                                break;
                            }
                        }
                    } else {
                        Node* ndashpn = ndash->children.front();
                        ndash->children.erase(ndash->children.begin());
                        K ndashkn = ndash->key.front();
                        ndash->key.erase(ndash->key.begin());

                        p->children.push_back(ndashpn);
                        p->key.push_back(ndashkn);
                        parentNode = p->parent;
                        for (int i=0; i<parentNode->key.size(); i++) {
                            if (parentNode->key[i] == value_) {
                                parentNode->key[i] = ndashkn;
                                break;
                            }
                        }
                    }
                }
            
                if (!ndash->leaf) {
                    for (int i=0; i<ndash->children.size(); i++) {
                        ndash->children[i]->parent = ndash;
                    }
                }
                if (!p->leaf) {
                    for (int i=0; i<p->children.size(); i++) {
                        p->children[i]->parent = p;
                    }
                }
                if (!parentNode->leaf) {
                    for (int i=0; i<parentNode->children.size(); i++) {
                        parentNode->children[i]->parent = parentNode;
                    }
                }
            } 
        }
    }
}

template <class K, class V>
void BpTree<K, V>::clear() {
    clear(root);
    root = nullptr;
}

template <class K, class V>
void BpTree<K, V>::clear(Node* p) {
    // Se p não for uma folha, deleta as children primeiro
    if (!p->leaf) {
        for(int i=0; i<p->children.size(); i++) {
            clear(p->children[i]);
        }
    }
    delete p;
}