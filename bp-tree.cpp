#include "bp-tree.h"
#include <random>
#include <iostream>
#include <vector>

int main() {
    BpTree<int, int>* tree = new BpTree<int, int>(5);
    for (int i=0; i<20; i++) {
        tree->insert(i, i);
    }

    vector<int> respostaFindRange = tree->findRange(10, 17);
    for (int x : respostaFindRange) {
        cout << x << endl;
    }

    for (int i=0; i<20; i++) {
        tree->remove(i);
    }
}