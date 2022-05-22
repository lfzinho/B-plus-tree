#include "bp-tree.h"
#include <random>

int main() {
    BpTree<int>* tree = new BpTree<int>(5);
    srand(0);
    for (int i=0; i<20; i++) {
        int key = rand()%20;
        cout << i << endl;
        tree->insert(i);
    }
}