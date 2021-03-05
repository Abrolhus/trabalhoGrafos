#ifndef DISJOINTSETFOREST_H
#define DISJOINTSETFOREST_H
#include <iostream>

class DisjointSetForest {

    public:
        DisjointSetForest(int size);
        int find(int);
        void setUnion(int v, int w);
        void print();
    private:
        int *parents;
        int size;
};

#endif
