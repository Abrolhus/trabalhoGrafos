#include "DisjointSetForest.h"
DisjointSetForest::DisjointSetForest(int size){
    this->parents = new int(size);
    this->size = size;
    for(int i = 0; i < size; i++){
        parents[i] = -1;
    }
}
int DisjointSetForest::find(int v){
    int parent = this->parents[v];
    int parentIndex = -1;
    while(parent >= 0){
        parentIndex = parent;
        parent = this->parents[parent];
    }
    return parentIndex;
}
void DisjointSetForest::setUnion(int v, int w){
    int vParent = this->find(v);
    int wParent = this->find(w);
    if(this->parents[vParent] <= this->parents[wParent]){ // e.g vParent = -6, wParent = -8, wParent(-8) will be the parent
        wParent = vParent;
        this->parents[vParent] += this->parents[wParent]; // -8 + -6 = -14 aka 14 filhos
    }
    else{
        vParent = wParent;
        this->parents[wParent] += this->parents[vParent]; // -8 + -6 = -14 aka 14 filhos
    }
}
void DisjointSetForest::print(){
    for(int i = 0; i < this->size; i++){
        std::cout << i << ": " << this->parents[i] << ". Paizao: " << this->find(i) << std::endl;
    }
}
