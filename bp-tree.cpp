#include "bp-tree.h"
#include <random>
#include <iostream>
#include <vector>

int main() {
    // testa o insert
    BpTree<int, int>* tree = new BpTree<int, int>(5);
    for (int i=0; i<20; i++) {
        tree->insert(i, i);
    }

    // testa o find
    for (int i=0; i<20; i++) {
        cout << "Node:" << i << " | Value found:" << tree->find(i) << endl;
    }

    // testa o FindRange
    vector<int> respostaFindRange = tree->findRange(10, 17);
    for (int i=0; i<respostaFindRange.size(); i++) {
        std::cout << "Found in range: " << respostaFindRange[i] << std::endl;
    }

    // testa o remove
    tree->remove(0);
    try {
        tree->find(0);
    } catch (const char* msg) {
        std::cout << msg << std::endl;
    }

    // testa o clear
    tree->clear();
    try {
        tree->find(1);
    } catch (const char* msg) {
        std::cout << msg << std::endl;
    }

    return 0;
}