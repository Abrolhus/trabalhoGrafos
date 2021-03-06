#include "DisjointSetForest.h"
DisjointSetForest::DisjointSetForest(int size){
    this->parents = std::vector<int>(size);
    this->size = size;
    for(int i = 0; i < size; i++){
        parents[i] = -1;
    }
}
int DisjointSetForest::find(int v){
    if(v >= this->size){
//         std::cout << "out of bounds!" << std::endl;
  //       std::cout << "out of bounds!" << std::endl;
        return -1;
    }
    int parent = this->parents[v];
    // std::cout << "parents:";
    // std::cout << parent << ", ";
    int parentIndex = -1;
    // return parent;
    while(parent >= 0){
        parentIndex = parent; // 2
        parent = this->parents[parent]; // -1
    }
    if(parentIndex < 0){
        return v;
    }
    return parentIndex;
}
void DisjointSetForest::setUnion(int v, int w){
    if(v >= this->size || w >= this->size){
        std::cout << "overflow!" << std::endl;
        return;
    }
    // std::cout << "parents[w]: " << parents[w] << ",, ";
    // std::cout << "parents[v]: " << parents[v] << ",, ";
    if(this->parents[v] <= this->parents[w]){ // e.g v = -6, w = -8, w(-8) will be the parent
        // w = v;
        this->parents[v] += this->parents[w]; // -8 + -6 = -14 aka 14 filhos
        this->parents[w] = v;
        // std::cout << "parente no source: " << this->parents[v] << std::endl;
    }
    else{
        // v = w;
        this->parents[w] += this->parents[v]; // -8 + -6 = -14 aka 14 filhos
        this->parents[v] = w;
        // std::cout << "parente no target: " << this->parents[v] << std::endl;
    }
}
void DisjointSetForest::print(){
    for(int i = 0; i < this->size; i++){
        std::cout << i << ": " << this->parents[i] << ". Paizao: " << this->find(i) << std::endl;
    }
}
