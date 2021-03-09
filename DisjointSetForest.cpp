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
        std::cout << i << ": ";
        // << this->parents[i] << "";
        this->printAllFathersOf(i);
        std::cout<< ". Paizao: " << this->find(i) << std::endl;
    }
}
bool DisjointSetForest::isDescendantOf(int v, int q){
/* checks if n is in Q`s "tree"
 */
    if(v >= this->size){
        std::cout << "out of bounds!" << std::endl;
        std::cout << "out of bounds!" << std::endl;
        return -1;
    }
    int parent = this->parents[v];
    // std::cout << "parents:";
    // std::cout << parent << ", ";
    int parentIndex = -1;
    while(parent >= 0){
        if(parent == q)
            return true;
        parent = this->parents[parent]; // -1
    }
    return false;
}
// std::vector<EdgeInfo> DisjointSetForest::getEdgesLeavingSubTree(int q, std::vector<EdgeInfo> edges){
//     /* returns all edges from a node in the q's subtree and a node not in q's subtree
//      * params: EdgeInfo vector, containing all avaliable edges
//      *
//      *
//      * basta que uma edge seja descendente de Q e outra nao. Claro que ambas tem que pertencer a mesma arvore
//      */
//     // auto fathersOfQ = getAllFathersOf(q);
//     std::vector<EdgeInfo> validEdges;
// <<<<<<< Updated upstream
//     for(auto edge : edges){
//         int edgeSource = edge.getNodeIdSource();
//         int edgeTarget = edge.getNodeIdTarget();
//         std::cout << "source: " << edgeSource << ", ";
//         std::cout << "target: " << edgeTarget << ", " << std::endl;
//         if(this->find(edgeSource) == this->find(edgeTarget)){
// =======
//     //std::cout << "Tamanho1: " << edges.size() << std::endl;
//     for(auto edge : edges){
//         int edgeSource = edge.getNodeIdSource();
//         int edgeTarget = edge.getNodeIdTarget();
//         //std::cout << "source: " << edgeSource << ", ";
//         //std::cout << "target: " << edgeTarget << ", " << std::endl;
//         if(this->find(edgeSource) == this->find(edgeTarget)){
//             //std::cout << "Entrou 1if" << std::endl;
// // >>>>>>> Stashed changes
// //             if(this->isDescendantOf(edgeSource, q) != this->isDescendantOf(edgeTarget, q)){ // xor    (true, false) or (false true)
// //                 validEdges.push_back(edge);
// //             }
// //         }
// //     }
// // <<<<<<< Updated upstream
// // =======
// //     //std::cout << "Tamanho: " << validEdges.size() << std::endl;
// // >>>>>>> Stashed changes
//     return validEdges;
//         // int firstCommonFather = getFirstCommonFather(edgeSource, edgeTarget);
//         // if(firstCommonFather == -1){
//             // std::cout << "uai";
//             // continue;
//         // }
//         // for(int i = 0; i < fathersOfQ.size(); i++){
//             // if(fathersOfQ[i] == firstCommonFather){
//                 // validEdges.push_back(edge);
//                 // break;
//             // }
//         // }
//     // }
// }
std::vector<int> DisjointSetForest::getAllFathersOf(int v){
    std::vector<int> vFathers;
    int parent = this->parents[v];
    // std::cout << "parents:";
    // std::cout << parent << ", ";
    int parentIndex = -1;
    // return parent;
    while(parent >= 0){
        vFathers.push_back(parent);
        parentIndex = parent; // 2
        parent = this->parents[parent]; // -1
    }
    if(parentIndex < 0){
        // return v;
    }
    return vFathers; // empty if v is the parent of everyone in a tree
}
int DisjointSetForest::getFirstCommonFather(int q, int v){
  // faster if Q is 'higher than v'
    std::vector<int> fathersOfQ = this->getAllFathersOf(q);
    for(int i = fathersOfQ.size() - 1; i >= 0; i--){ // se v for pai de q
        std::cout << "`" << fathersOfQ.at(i);
        if(fathersOfQ.at(i) == v){
            return v;
        }
    }
    int parent = this->parents[v];
    std::cout << v << ": ";
    // std::cout << "parents:";
    // std::cout << parent << ", ";
    int parentIndex = -1;
    // return parent;
    while(parent >= 0){
        parentIndex = parent; // 2
        std::cout << parent << ", ";
        if(parentIndex == q){ // se q for pai de v
            return q;
        }
        // for(int i = fathersOfQ.size() - 1; i >= 0; i--){ // procurando primeiro pai em comum
        for(int i = 0; i < fathersOfQ.size(); i++){ // procurando primeiro pai em comum
            if(fathersOfQ.at(i) == parentIndex){
                return parentIndex;
            }
        parent = this->parents[parentIndex]; // -1
        }
    }
    if(parentIndex < 0){
        return -1;
    }
    return parentIndex;
}
std::vector<int> DisjointSetForest::getAllTrees(){
    // returns all the full parents;
    std::vector<int> trees;
    for(int i = 0; i < this->parents.size(); i++){
        if(parents[i] < 0){
            trees.push_back(i);
            if(parents[i] == -this->size)
                return trees;
        }
    }
    return trees;
}
void DisjointSetForest::printAllFathersOf(int v){
    int parent = this->parents[v];
    int parentIndex = -1;
    // return parent;
    //
    std::cout<< "{";
    while(parent >= 0){
        std::cout << parent << "";
        parent = this->parents[parent]; // -1
        if(parent >= 0){
          std::cout << ", ";
        }
    }
    std::cout<< "}";

}
std::vector<EdgeInfo> DisjointSetForest::caminhoReuniaoFamiliar(int v, int w, int firstCommonFather){
    int vParent = v;
    int wParent = w;
    int prevNode = v;
    int parentIndex = -1;
    std::vector<EdgeInfo> edges;
    // return parent;
    //
    std::cout << "{";
    while (vParent >= 0 && vParent != firstCommonFather) {
        vParent = this->parents[vParent]; // -1
        edges.push_back(EdgeInfo(prevNode, vParent, 10000));
        prevNode = vParent;
  }
  prevNode = w;
    while (wParent >= 0 && wParent != firstCommonFather) {
        std::cout << wParent << "";
        wParent = this->parents[wParent]; // -1
        edges.push_back(EdgeInfo(prevNode, wParent, 10000));
        prevNode = wParent;
  }
  return edges;

}



