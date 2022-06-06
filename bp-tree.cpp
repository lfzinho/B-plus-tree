#include "bp-tree.h"
#include <random>

int main() {
    BpTree<int, int>* tree = new BpTree<int, int>(5);
    for (int i=0; i<20; i++) {
        cout << i << endl;
        tree->insert(i, i);
    }
}