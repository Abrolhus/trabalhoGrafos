#ifndef DISJOINTSETFOREST_H
#define DISJOINTSETFOREST_H
#include <iostream>
#include "EdgeInfo.h"
#include <vector>

class DisjointSetForest {

    public:
        DisjointSetForest(int size);
        int find(int);
        void setUnion(int v, int w);
        void print();
        bool isSomewhatSonOf(int n, int q);
        std::vector<EdgeInfo> getEdgesLeavingSubTree(int q, std::vector<EdgeInfo>);
        std::vector<int> getAllTrees();
        std::vector<int> getAllFathersOf(int);
        int getCommonFatherBetween(int B, int knownFatherOfB, int notSiblingOfB);
        int getFirstCommonFather(int q, int v);
        void printAllFathersOf(int v);
    private:
        std::vector<int> parents;
        int size;
};

#endif
