#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <set>
#include <queue>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/
// Struct used on Dijkstra's method for storing the distance/cost from Node X to node i;
typedef struct {
    Node* node;
    float pathCost;
    int previousId;
    bool wasVisited;
} NodeDist;
// bool compare (NodeDist n1, NodeDist n2){ return false;}
class compareCosts {
  bool reverse;
public:
  compareCosts(const bool &revparam = false) { reverse = revparam; }
  bool operator() (const NodeDist& lhs, const NodeDist& rhs) const
  {
    if (reverse) return (lhs.pathCost>rhs.pathCost);
    else return (lhs.pathCost<rhs.pathCost);
  }
};
// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    for(int i = 0; i < order; i++){
        this->insertNode(i);
    }
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}


Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    //confere se o grafo tem nodes
    if (first_node != nullptr)
    {
        //caso tenha, cria um novo node, aponta o ultimo pro novo e o novo de torna o ultimo
        Node* novo_node = new Node(id);
        last_node->setNextNode(novo_node);
        last_node = novo_node;
    }
    else
    {
        //caso nao tenha, cria um novo node e ele se torna o ultimo e o primeiro
        Node* novo_node = new Node(id);
        first_node = novo_node;
        last_node = novo_node;
    }

}

void Graph::insertEdge(int id, int target_id, float weight)
{
    //cria um ponteiro para o node desejado
    //cout << "inserindo " << id << "->" << target_id << endl;
    Node* p = getNode(id); // no 0

    // se o node existir, insere a edge com o target desejado
    if (p != nullptr)
    {
        p->insertEdge(target_id, weight);
    }

}

void Graph::removeNode(int id)
{
    //cria um ponteiro para o node desejado
    Node* p = getNode(id);

    //retorna caso nao exista o node desejado
    if (p == nullptr)
        return;
    else if (p == first_node) //se o node eh o primeiro, apenas faz o proximo ser o primeiro
        first_node = p->getNextNode();
    else
    {
        //caso seja um node do meio ou o ultimo, cria um ponteiro auxiliar
        Node* aux = first_node;

        //encontra o node anterior ao node desejado
        while (aux->getNextNode() != p)
            aux = aux->getNextNode();

        //se o no desejado for o ultimo, atualiza o ultimo para o anterior
        if (p == last_node)
            last_node = aux;

        //seta o proximo de anterior para o proximo do desejado
        aux->setNextNode(p->getNextNode());
    }

    //deleta o node desejado
    delete p;
}

bool Graph::searchNode(int id)
{
    //cria um ponteiro para o node desejado
    Node* p = getNode(id);

    //retorna falso caso nao encontre e verdadeiro caso encontre
    if (p == nullptr)
        return false;
    else
        return true;

}

Node *Graph::getNode(int id)
{
    //cria ponteiro para percorrer a lista de nodes
    Node* p = first_node;

    //encontra o node com o id desejado
    while (p != nullptr)
    {
        if (p->getId() == id)
            break;

        p = p->getNextNode();
    }

    //retorna o node ou null caso nao encontre
    return p;
}


//Function that prints a set of edges belongs breadth tree

// void Graph::breadthFirstSearch(ofstream &output_file){

// }



// float Graph::floydMarshall(int idSource, int idTarget){

// }



float Graph::dijkstra(int idSource, int idFinal){
    map<int, NodeDist> allNodes;
    priority_queue<NodeDist, vector<NodeDist>, compareCosts> unvisitedNodes;
    auto sourceNode = this->getNode(idSource);
    for(Node* p = this->getFirstNode(); p != nullptr; p = p->getNextNode()){
        if(p == sourceNode){
            allNodes.insert({p->getId(), {p, INF, -1, false} });
            continue;
        }
        unvisitedNodes.push({p, INF, -1, false});
        allNodes.insert({p->getId(), {p, INF, -1, false} });
    }
    auto currentNode = sourceNode;
    while(1){
        cout << "currentNode: " << currentNode->getId() << endl;
        for(auto edge = currentNode->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            int targetId = edge->getTargetId(); // O(1)
            cout << "id:" << targetId;
            NodeDist* nodeInfo = &allNodes.at(targetId); // O(1)
            if(nodeInfo->wasVisited){// if node was no "visited" //  // O(1)
                continue;
            }
            cout << "(pathCost:" << nodeInfo->pathCost << ")";
            cout << "(" <<  currentNode->getWeight() + nodeInfo->node->getWeight() << ") ";
            if(nodeInfo->pathCost >= currentNode->getWeight() + nodeInfo->node->getWeight()){
                nodeInfo->pathCost = currentNode->getWeight() + nodeInfo->node->getWeight();
                nodeInfo->previousId = currentNode->getId();
                cout << "[NEWPATHCOST!:" << nodeInfo->pathCost << "]";
            }
        }
        allNodes.at(currentNode->getId()).wasVisited = true;
        unvisitedNodes.pop();
        currentNode = unvisitedNodes.top().node;
        if(currentNode == this->getNode(idFinal))
            return unvisitedNodes.top().pathCost;
    }
    return 0.0f;
}

// //function that prints a topological sorting
// void topologicalSorting(){

// }

// void breadthFirstSearch(ofstream& output_file){

// }

// Graph* Graph::getVertexInduced(int* listIdNodes, ifstream& input_file)
// {
//     int idNodeSource;
//     int idNodeTarget;
//     int order;
//     int numEdges;

//     input_file >> order;


//     Graph* grafoInduzido = new Graph(order, 0, 0, 0);

//     while(input_file >> idNodeSource >> idNodeTarget) {

//         grafoInduzido->insertEdge(idNodeSource-1, idNodeTarget-1, 0);

//     }

//     Node* p = grafoInduzido->first_node;
//     Edge* e = nullptr;


//     while (p != nullptr)
//     {
//         if (!auxGetVertexInduced(p->getId(), listIdNodes, grafoInduzido))
//         {
//             grafoInduzido->removeNode(p->getId());
//         }
//         p = p->getNextNode();
//     }

//     p = grafoInduzido->first_node;


//     while (p != nullptr)
//     {
//         e = p->getFirstEdge();
//         while (e != nullptr)
//         {
//             if (!auxGetVertexInduced(e->getTargetId(), listIdNodes, grafoInduzido))
//             {
//                 if (grafoInduzido->getNode(e->getTargetId()) == nullptr)
//                 {
//                     p->removeAresta(e->getTargetId());
//                 }
//             }

//             e = e->getNextEdge();
//         }

//         p = p->getNextNode();
//     }

//     return grafoInduzido;
// }


// Graph* agmKuskal(){

// }
// Graph* agmPrim(){

// }

// bool Graph::auxGetVertexInduced (int id, int* listIdNodes, Graph* grafo)
// {
//     Node *p = grafo->first_node;

//     while (p != nullptr)
//     {
//         for (int i = 0; listIdNodes[i] != -1; i++)
//         {
//             if(p->getId() == listIdNodes[i])
//             {
//                 return true;
//             }
//         }

//         p = p->getNextNode();
//     }

//     return false;
// }

void Graph::print(){
    cout << "imprimindo grafo" << endl;
    cout << this->getOrder() << endl;
    Node* p = this->first_node;
    while(p != nullptr){
        cout << p->getId() +1 << ": ";
        Edge* e = p->getFirstEdge();
        while(e != nullptr){
            cout << "->" << e->getTargetId() +1 << " ";
            e = e->getNextEdge();
        }
        cout << endl;
        p = p->getNextNode();
    }

    return;

}
void Graph::escreverEmArquivo(ofstream& output_file){

    output_file << this->getOrder() << endl;
    Node* p = this->first_node;
    while(p != nullptr){
        output_file << p->getId() +1 << ": ";
        Edge* e = p->getFirstEdge();
        while(e != nullptr){
            output_file << p->getId() + 1<< "->" << e->getTargetId() +1 << ", " ;
            e = e->getNextEdge();
        }
        output_file << endl;
        p = p->getNextNode();
    }



    return;
}
