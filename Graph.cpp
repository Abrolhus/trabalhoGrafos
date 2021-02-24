#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "QNode.h"
#include "ChainedQueue.h"
#include "EdgeInfo.h"
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
#include <vector>
#define INFINITO 999;

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
  bool operator() (const NodeDist* lhs, const NodeDist* rhs) const
  {
    if (reverse) return (lhs->pathCost<rhs->pathCost);
    else return (lhs->pathCost>rhs->pathCost);
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
    //cria um ponteiro para o node desejado e o um auxiliar para o node alvo da aresta
    Node* p = getNode(id);
    Node* aux = getNode(target_id);

    //confere se os nodes existem
    if (p != nullptr && aux != nullptr){

        //confere se a aresta já existe
        if (!p->searchEdge(target_id)){

            //caso o node exista mas a aresta nao, insere a aresta
            p->insertEdge(target_id, weight);
            this->number_edges++;

            // se o grafo for nao-direcionado e nao houver aresta de
            if (this->directed == 0 && !aux->searchEdge(id))
            {
                //insere a aresta de volta
                aux->insertEdge(id, weight);
                this->number_edges++;
            }
        }
    }
}

void Graph::insertEdgePreguicoso(int id, int target_id, float weight)
{
    //cria um ponteiro para o node desejado e o um auxiliar para o node alvo da aresta
    Node* p = getNode(id-1);
    Node* aux = getNode(target_id-1);

    //confere se os nodes existem
    if (p != nullptr && aux != nullptr){

        //confere se a aresta já existe
        if (!p->searchEdge(target_id-1)){

            //caso o node exista mas a aresta nao, insere a aresta
            p->insertEdge(target_id-1, weight);
            this->number_edges++;

            // se o grafo for nao-direcionado e nao houver aresta de
            if (this->directed == 0 && !aux->searchEdge(id-1))
            {
                //insere a aresta de volta
                aux->insertEdge(id-1, weight);
                this->number_edges++;
            }
        }
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

bool Graph::auxIsCyclic(int nodeId, bool isVisited[], int parentId)
{
    isVisited[nodeId] = true;
    Node *p = getNode(nodeId);
    Edge *e = p->getFirstEdge();

    while (e != nullptr)
    {
        if(!isVisited[e->getTargetId()])
        {
            if (auxIsCyclic(e->getTargetId(), isVisited, nodeId))
            {
                return true;
            }
        }
        else if (e->getTargetId() != parentId)
        {
            return true;
        }

        e = e->getNextEdge();
    }


    return false;
}

bool Graph::isCyclic()
{
    bool *isVisited = new bool[this->getOrder()];
    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = false;
    }

    Node *p = first_node;
    while(p != nullptr)
    {
        if(!isVisited[p->getId()])
        {
            if(auxIsCyclic(p->getId(), isVisited, -1))
            {
                return true;
            }
        }


        p = p->getNextNode();
    }

    return false;
}

bool Graph::auxIsCyclicDirected(int nodeId, bool isVisited[], bool *isContainedRecusirve)
{
    Node *p = this->getNode(nodeId);
    Edge *e = p->getFirstEdge();

    if (!isVisited[nodeId])
    {
        isVisited[nodeId] = true;
        isContainedRecusirve[nodeId] = true;

        while (e != nullptr)
        {
            if(!isVisited[e->getTargetId()] && auxIsCyclicDirected(e->getTargetId(), isVisited, isContainedRecusirve))
            {
                return true;
            }
            else if (isContainedRecusirve[e->getTargetId()])
            {
                return true;
            }

            e = e->getNextEdge();
        }

    }

    isContainedRecusirve[nodeId] = false;
    return false;
}

bool Graph::isCyclicDirected()
{
    bool *isVisited = new bool[this->getOrder()];
    bool *isContainedRecusirve = new bool[this->getOrder()];
    Node *p = first_node;

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = false;
        isContainedRecusirve[i] = false;
    }

    while (p != nullptr)
    {
        if (auxIsCyclicDirected(p->getId(), isVisited, isContainedRecusirve))
        {
            return true;
        }

        p = p->getNextNode();
    }

    return false;
}


//Function that prints a set of edges belongs breadth tree


void Graph::breadthFirstSearch(int idFirstNode){
    int cont = 1;
    int ordem[this->getOrder()];
    Node *p = this->getNode(idFirstNode - 1);
    Edge *e = nullptr;
    ChainedQueue *fila = new ChainedQueue();

    for (int i = 0; i < this->getOrder(); i++)
    {
        ordem[i] = 0;
    }

    fila->queueUp(p->getId());
    ordem[p->getId()] = cont++;

    while (!fila->isEmpty())
    {
        p = this->getNode(fila->queueDown());
        e = p->getFirstEdge();
        while (e != nullptr)
        {
            if(ordem[e->getTargetId()] == 0)
            {
                ordem[e->getTargetId()] = cont++;
                fila->queueUp(e->getTargetId());
            }
            e = e->getNextEdge();
        }
    }


    int aux[this->getOrder()];
    cout << endl;
    for (int i = 0; i < this->getOrder(); i++)
    {
        aux[ordem[i] - 1] = i + 1;
    }

    cout << "Ordem de descoberta: ";
    for (int i = 0; i < this->getOrder(); i++)
    {
        cout << aux[i]  << " ";
    }

}



float Graph::floydMarshall(int idSource, int idTarget){
    vector<vector<float>> edgeMatrix(this->getOrder(), vector<float>(this->getOrder(), INF)); // matriz com os pesos das edges de todo noh (i) para cada outro noh (j)
    for(int i = 0; i < this->getOrder(); i++){
        for(int j = 0; j < this->getOrder(); j++){
            cout << edgeMatrix[i][j] << ", ";
        }
        cout << endl;
    }
    for(auto node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){ // O(n^2)
        edgeMatrix[node->getId()][node->getId()] = 0;
        for(auto edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            edgeMatrix[node->getId()][edge->getTargetId()] = edge->getWeight();
        }

    }
    for(int k = 0; k < this->getOrder(); k++){
        //percorrer matriz:
        for(int i = 0; i < this->getOrder(); i++){
            for(int j = 0; j < this->getOrder(); j++){
                edgeMatrix[i][j] = min(edgeMatrix[i][j], edgeMatrix[i][k] + edgeMatrix[k][j]);
            }
        }
    }
    cout << "Matriz final do Floyd" << endl;
    for(int i = 0; i < this->getOrder(); i++){
        for(int j = 0; j < this->getOrder(); j++){
            cout << edgeMatrix[i][j] << ", ";
        }
        cout << endl;
    }
    return edgeMatrix[idSource][idTarget];
}



float Graph::dijkstra(int idSource, int idFinal){
    map<int, NodeDist> allNodes;
    priority_queue<NodeDist*, vector<NodeDist*>, compareCosts> unvisitedNodes;
    Node* sourceNode = this->getNode(idSource);
    for(Node* p = this->getFirstNode(); p != nullptr; p = p->getNextNode()){
        if(p == sourceNode){
            allNodes.insert({p->getId(), {p, 0, -1, false} });
            continue;
        }
        allNodes.insert({p->getId(), {p, INF, -1, false} });
        // unvisitedNodes.push({p, INF, -1, false});
        unvisitedNodes.push(&allNodes.at(p->getId()));
    }
    for(auto& item : allNodes){
        cout << item.first << ":" << item.second.wasVisited<< ", "<< item.second.pathCost << endl;
    }

    // auto currentNode = sourceNode;
    NodeDist* firstNodeInfo = &allNodes.at(sourceNode->getId());
    NodeDist* currentNodeInfo = &allNodes.at(sourceNode->getId());
    while(1){




        auto currentNode = currentNodeInfo->node;
        cout << "currentNode: " << currentNode->getId() +1<< "custoAteEle:" << currentNodeInfo->pathCost<< endl;
        for(auto edge = currentNode->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            int targetId = edge->getTargetId(); // O(1)
            NodeDist* nodeInfo = &allNodes.at(targetId); // O(1) if(nodeInfo->wasVisited){// if node was no "visited" //  // O(1) continue; }
            cout << "     id:" << targetId +1 << ", custoAntigo: " << nodeInfo->pathCost;
            if(nodeInfo->pathCost >= (edge->getWeight() + currentNodeInfo->pathCost)){
                cout << " _custoDaEdge:" << edge->getWeight();
                nodeInfo->pathCost = edge->getWeight() + currentNodeInfo->pathCost;
                nodeInfo->previousId = currentNode->getId();
                cout << " _novoCusto:" << nodeInfo->pathCost << ", "<< endl;
            }
        }
        cout << endl;
        allNodes.at(currentNode->getId()).wasVisited = true;
        unvisitedNodes.pop();
        currentNodeInfo = (unvisitedNodes.top());
        if(currentNode == this->getNode(idFinal)){
            cout << currentNode->getId() +1<< "<-";

            NodeDist* p = currentNodeInfo;
            while(p != firstNodeInfo){
                cout << p->previousId +1 << "<-";
                p = &allNodes.at(p->previousId);
            }

            return unvisitedNodes.top()->pathCost;
        }
    }
    return 0.0f;
}

int Graph::cheapestNeighbor(int neighborPath[], int visitedNodes[])
{   
    int min = INFINITO;
    int cheapestNeighborId;

    for (int i = 0; i < this->getOrder(); i++)
    {
        if (!checkContainsId(i, visitedNodes, this->getOrder()) && neighborPath[i] < min)
        {
            min = neighborPath[i];
            cheapestNeighborId = i;
        }
    }

    return cheapestNeighborId;
}

void Graph::updateNeighborPath(int id, int neighborPath[])
{
    Node *p = this->getNode(id);
    Edge *e = p->getFirstEdge();

    for (int i = 0; i < this->getOrder(); i++)
    {
        e = p->getFirstEdge();
        while (e != nullptr)
        {
            if (e->getTargetId() == i && neighborPath[i] > e->getWeight())
            {
                neighborPath[i] = e->getWeight();
            }
            e = e->getNextEdge();
        }
    }
}

Graph* Graph::prim(){
    int visitedNodes[this->getOrder()];
    int neighborPath[this->getOrder()];
    int nextNodeId = 0;
    int index = 0;
    int idAlvo;
    int minimum = INFINITO;
    int totalMiniumWeight = 0;
    Node *p = nullptr;
    Edge *e = nullptr;
    Graph *grafoSolucao = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    for (int i = 0; i < this->getOrder(); i++)
    {
        visitedNodes[i] = -1;
        neighborPath[i] = INFINITO;
    }

    neighborPath[0] = 0;


    while (visitedNodes[this->getOrder() - 1] == -1)
    {
        nextNodeId = cheapestNeighbor(neighborPath, visitedNodes);
        updateNeighborPath(nextNodeId, neighborPath);
        minimum = INFINITO;

        if (nextNodeId != 0)
        {
            p = this->getNode(nextNodeId);
            e = p->getFirstEdge();
            while (e != nullptr)
            {
                if (checkContainsId(e->getTargetId(), visitedNodes, this->getOrder()) && e->getWeight() < minimum)
                {
                    minimum = e->getWeight();
                    idAlvo = e->getTargetId(); 
                }
                e = e->getNextEdge();
            }

            grafoSolucao->insertEdge(p->getId(), idAlvo, minimum);
            totalMiniumWeight += minimum;
        }

        visitedNodes[index] = nextNodeId;
        index++;
    }


    return grafoSolucao;
}

// //function that prints a topological sorting
// void topologicalSorting(){

// }

// void breadthFirstSearch(ofstream& output_file){

// }

// Graph* agmKuskal(){

// }
// Graph* agmPrim(){

// }

bool Graph::checkContainsId(int id, int nodeList[], int listLength)
{
    for (int i = 0; i < listLength; i++)
    {
        if (id == nodeList[i])
            return true;
    }

    return false;
}

int Graph::listSortEdges(int isVisited[], EdgeInfo *graphEdges)
{
    Node *p = first_node;
    Edge *e = p->getFirstEdge();
    int i = 0;
    int visNode = 0;

    //adiciona todas as arestas do grafo no TAD EdgeInfo
    while (p != nullptr)
    {
        //adiciona nós ja visitados para evitar de inserir arestas duas vezes no caso de grafos nao orientados
        isVisited[visNode] = p->getId();
        e = p->getFirstEdge();

        while (e != nullptr)
        {
            //se orientado, somente adiciona aresta
            if(this->getDirected())
            {
                graphEdges[i].setNodeIdSource(p->getId());
                graphEdges[i].setNodeIdTarget(e->getTargetId());
                graphEdges[i].setEdgeWeight(e->getWeight());
                i++;
            }
            //checha se o no ja foi target ja foi visitado para nao inserir a aresta de volta em casos de grafos nao orientaos
            else if (!checkContainsId(e->getTargetId(), isVisited, this->getOrder()))
            {
                graphEdges[i].setNodeIdSource(p->getId());
                graphEdges[i].setNodeIdTarget(e->getTargetId());
                graphEdges[i].setEdgeWeight(e->getWeight());
                i++;
            }

            e = e->getNextEdge();
        }

        visNode++;
        p = p->getNextNode();
    }

    //guarda a quantidade de elementos adicionados
    int listSize = i;

    //depois de adicionadas, as organiza de mais barata a mais cara
    EdgeInfo aux;

    for (int j = 0; j < listSize; j++)
    {
        for (int k = j + 1; k < listSize; k++)
        {
            if(graphEdges[j].getEdgeWeight() > graphEdges[k].getEdgeWeight())
            {
                aux = graphEdges[j];
                graphEdges[j] = graphEdges[k];
                graphEdges[k] = aux;
            }
        }

    }

    return listSize;
}

Graph* Graph::kruskal()
{
    EdgeInfo *graphEdges = new EdgeInfo[this->getNumberEdges()];
    int isVisited[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }

    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges);

    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    EdgeInfo *edgeSolution = new EdgeInfo[listSize];
    int solutionSize = 0;
    Node *p = nullptr;

    for (int i = 0; i < listSize; i++)
    {
        //insere a aresta no grafo auxiliar
        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());

        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil())
        {
            edgeSolution[solutionSize] = graphEdges[i];
            solutionSize++;
        }
        //caso forme um ciclo, retire a aresta recem adicionada
        else
        {
            //ponteiro aponta pra no de saida e remove a aresta
            p = aux->getNode(graphEdges[i].getNodeIdSource());
            p->removeEdge(graphEdges[i].getNodeIdTarget(), this->directed, this->getNode(graphEdges[i].getNodeIdTarget()));

            //ponteiro aponta para no de chegada e remove a aresta
            p = aux->getNode(graphEdges[i].getNodeIdTarget());
            p->removeEdge(graphEdges[i].getNodeIdSource(), this->directed, this->getNode(graphEdges[i].getNodeIdSource()));

        }
    }

    cout << endl;

    delete [] graphEdges;
    delete [] edgeSolution;
    return aux;
}

bool Graph::isCyclicUtil()
{
    if (this->directed)
        return this->isCyclicDirected();
    else
        return this->isCyclic();
}

Graph* Graph::getVertexInduced(int* listIdNodes){

    //Cria grafo vazio
    Graph* grafoInduzido = new Graph(0, this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    //Cria ponteiro pra vertices para percorrer o grafo
    Node* p = this->getFirstNode();

    //Cria ponteiro pra aresta para percorrer o vertice
    Edge* e = nullptr;

    //Para cada vertice do grafo
    while (p != nullptr)
    {
        //reseta ponteiro da aresta
        e = nullptr;

        //confere se o vértice esta contido na lista de vertices
        if (auxGetVertexInduced(p->getId(), listIdNodes))
        {
            //caso esteja adiciona vertice ao novo grafo
            grafoInduzido->insertNode(p->getId());
            grafoInduzido->order++;

            //percorre as arestas do vertice
            e = p->getFirstEdge();
            while (e != nullptr)
            {
                //confere se o vertice para qual a aresta vai esta contido na lista
                if (auxGetVertexInduced(e->getTargetId(), listIdNodes))
                {
                    //caso esteja, adiciona aresta ao vertice
                    grafoInduzido->getNode(p->getId())->insertEdge(e->getTargetId(), 0);
                }
                e = e->getNextEdge();
            }
        }
        p = p->getNextNode();
    }

    //retorna novo grafo induzido pelos vertices da lista
    return grafoInduzido;
}


bool Graph::auxGetVertexInduced (int id, int* listIdNodes)
{
    //testa se o vertice esta contido na lista de vertices
    for (int i = 0; listIdNodes[i] != -1; i++)
        {
        if(id == listIdNodes[i])
        {
            return true;
        }
    }

    return false;
}

void Graph::print(){
    cout << "imprimindo grafo" << endl;
    cout << "Ordem: " << getOrder() << endl;
    Node* p = first_node;
    while(p != nullptr){
        cout << p->getId() +1 << ": ";
        Edge* e = p->getFirstEdge();
        while(e != nullptr){
            cout << "->" << e->getTargetId()+1 << " ";
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
