#ifndef DISJOINTSETFOREST_H
#define DISJOINTSETFOREST_H
#include <iostream>
#include <vector>

class DisjointSetForest {

    public:
        DisjointSetForest(int size);
        int find(int);
        void setUnion(int v, int w);
        void print();
    private:
        std::vector<int> parents;
        int size;
};

#endif
