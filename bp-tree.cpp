#include "bp-tree.h"

int main() {
    BpTree<int>* tree = new BpTree<int>(5);
    tree->insert(2);
    tree->insert(3);
    bool a = tree->find(3);
    a = tree->find(6);
    a = tree->find(2);
}