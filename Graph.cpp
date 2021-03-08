#include "Graph.h"
#include <algorithm>
#include <stdlib.h>
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
#include <climits>
#include <chrono>
#include "DisjointSetForest.h"
#define INFINITO 9999;

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
bool compareEdges(EdgeInfo lhs, EdgeInfo rhs){
    return (lhs.getEdgeWeight()<rhs.getEdgeWeight());
}

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
    Node* p = getNode(id);
    Node* aux = getNode(target_id);

    p->incrementOutDegree();
    aux->incrementInDegree();

    //confere se os nodes existem
    if (p != nullptr && aux != nullptr){

        //confere se a aresta já existe
        if (!p->searchEdge(target_id)){

            //caso o node exista mas a aresta nao, insere a aresta
            p->insertEdge(target_id, weight);
            //cout << "Aresta: " << id <<  "->" << target_id << endl;
            this->number_edges++;

            // se o grafo for nao-direcionado e nao houver aresta de volta
            if (this->directed == 0 && !aux->searchEdge(id))
            {
                //insere a aresta de volta
                aux->insertEdge(id, weight);
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

bool Graph::isConnected()
{
    int ordem[this->getOrder()];
    this->breadthFirstSearch(0, ordem);
    for (int i = 0; i < this->getOrder(); i++)
    {
        if (ordem[i] == -1)
        {
            return false;
        }
    }
    return true;
}

void Graph::breadthFirstSearch(int idFirstNode, int ordem[]){
    int cont = 1;
    Node *p = this->getNode(idFirstNode);
    Edge *e = nullptr;
    ChainedQueue *fila = new ChainedQueue();

    for (int i = 0; i < this->getOrder(); i++)
    {
        ordem[i] = -1;
    }

    fila->queueUp(p->getId());
    ordem[p->getId()] = cont++;

    while (!fila->isEmpty())
    {
        p = this->getNode(fila->queueDown());
        e = p->getFirstEdge();
        while (e != nullptr)
        {
            if(ordem[e->getTargetId()] == -1)
            {
                ordem[e->getTargetId()] = cont++;
                fila->queueUp(e->getTargetId());
            }
            e = e->getNextEdge();
        }
    }

    // int aux[this->getOrder()];
    // cout << endl;
    // for (int i = 0; i < this->getOrder(); i++)
    // {
    //     cout << ordem[i]  << " ";
    //     aux[ordem[i] - 1] = i;

    //     if (ordem[i] == -1)
    //     {
    //         isConnected = false;
    //     }
    // }

    // cout << "Ordem de descoberta: ";
    // for (int i = 0; i < this->getOrder(); i++)
    // {
    //     cout << aux[i]  << " ";
    // }

    // return isConnected;
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

void Graph::connectionsFloyd(){
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
        cout << "[\t";
        for(int j = 0; j < this->getOrder(); j++){
            cout << edgeMatrix[i][j] << "\t";
        }
        cout << "]" << endl;
    }

    int exentricidade[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        exentricidade[i] = -1;
    }

    for (int i = 0; i < this->getOrder(); i++){
        for (int j = 0; j < this->getOrder(); j++){
            if (edgeMatrix[i][j] > exentricidade[i]){
                exentricidade[i] = edgeMatrix[i][j];
            }
        }
    }

    cout << endl;
    cout << "Exentricidade dos vértices: " << endl;
    cout << "v: [\t";
    for (int i = 0; i < this->getOrder(); i++){
        cout << i + 1 << "\t";
    }
    cout << "]" << endl;
    cout << "e: [\t";
    for (int i = 0; i < this->getOrder(); i++){
        cout << exentricidade[i] << "\t";
    }
    cout << "]" << endl;

    int raio = INFINITO;
    int diametro = -1;

    for (int i = 0; i < this->getOrder(); i++)
    {
        if (exentricidade[i] > diametro)
        {
            diametro = exentricidade[i];
        }

        if (exentricidade[i] < raio)
        {
            raio = exentricidade[i];
        }
    }

    cout << "Diametro: " << diametro << endl;
    cout << "Raio: " << raio << endl;

    cout << "Vertices centrais: ";
    for (int i = 0; i < this->getOrder(); i++)
    {
        if (exentricidade[i] == raio)
        {
            cout << i + 1 << " ";
        }
    }
    cout << endl;

    cout << "Vertices perifericos: ";
    for (int i = 0; i < this->getOrder(); i++)
    {
        if (exentricidade[i] == diametro)
        {
            cout << i + 1 << " ";
        }
    }
    cout << endl;

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

void Graph::topologicalSorting(){

    if (!this->getDirected() && this->isCyclicUtil())
    {
        cout << endl << "Ordenacao Topologica impossivel: grafo nao-direcionado ou ciclico" << endl;
        return;
    }

    ChainedQueue *DAGQueue = new ChainedQueue();

    Node *p = this->getFirstNode();
    while (p != nullptr)
    {
        if (p->getInDegree() == 0)
        {
            DAGQueue->queueUp(p->getId());
        }
        p = p->getNextNode();
    }

    int cont = 0;
    int DAG[this->getOrder()];
    Edge *e = nullptr;

    while (!DAGQueue->isEmpty())
    {
        p = this->getNode(DAGQueue->queueDown());
        DAG[cont] = p->getId();
        cont++;

        e = p->getFirstEdge();
        while (e != nullptr)
        {
            p->decrementOutDegree();
            this->getNode(e->getTargetId())->decrementInDegree();

            if (this->getNode(e->getTargetId())->getInDegree() == 0)
            {
                DAGQueue->queueUp(e->getTargetId());
            }
            e = e->getNextEdge();
        }
    }

    cout << endl << "Ordenacao Topologica do grafo direcionado acliclo: ";
    for (int i = 0; i < this->getOrder(); i++)
    {
        cout << DAG[i] + 1 << " ";

    }
    cout << endl;
}

bool Graph::checkContainsId(int id, int nodeList[], int listLength)
{
    for (int i = 0; i < listLength; i++)
    {
        if (id == nodeList[i])
            return true;
    }

    return false;
}

int Graph::listSortEdges(int isVisited[], vector<EdgeInfo>& graphEdges)
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
    std::sort(graphEdges.begin(), graphEdges.end(), compareEdges);

    // for (int j = 0; j < listSize; j++)
    // {
        // // TODO: botar um quicksor aq
        // for (int k = j + 1; k < listSize; k++)
        // {
            // if(graphEdges[j].getEdgeWeight() > graphEdges[k].getEdgeWeight())
            // {
                // aux = graphEdges[j];
                // graphEdges[j] = graphEdges[k];
                // graphEdges[k] = aux;
            // }
        // }
//
    // }

    return listSize;
}

Graph* Graph::kruskal()
{
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges(), EdgeInfo());
    int isVisited[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }

    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges);
    for(auto edge : graphEdges){
        cout << edge.getEdgeWeight() << ", ";
    } cout << endl;

    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    auto edgeSolution = vector<EdgeInfo>(listSize);
    int solutionSize = 0;
    Node *p = nullptr;

    int weight = 0;
    for (int i = 0; i < listSize; i++)
    {
        //insere a aresta no grafo auxiliar
        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());

        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil())
        {
            edgeSolution[solutionSize] = graphEdges[i];
            //cout << "peso antes: " << weight << endl;
            weight += graphEdges[i].getEdgeWeight();
            //cout << "peso depois: " << weight << endl;
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

    cout << "peso: " << weight << endl;
    cout << endl;

    // delete [] graphEdges;
    return aux;
}

Graph* Graph::kruskalRestritivo(int grauRestricao)
{
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
    int isVisited[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }
    cout << "isVisited: ";
    for (int i = 0; i < this->getOrder(); i++)
        cout << isVisited[i] << ", ";
    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges); //O(e*v)
    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    EdgeInfo *edgeSolution = new EdgeInfo[listSize];
    int solutionSize = 0;
    Node *p = nullptr;
    int weight = 0;
    cout << "ListSize: " << listSize << endl;
    for (int i = 0; i < listSize; i++)
    {
        cout << graphEdges[i].getNodeIdSource() << "->" << graphEdges[i].getNodeIdTarget() << " (" << graphEdges[i].getEdgeWeight() << ")" << endl;
        //insere a aresta no grafo auxiliar
        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());

        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil() && aux->getNode(graphEdges[i].getNodeIdSource())->getDegree() <= grauRestricao
        					 && aux->getNode(graphEdges[i].getNodeIdTarget())->getDegree() <= grauRestricao)
        {
            //cout << "peso antes: " << weight << endl;
            weight += graphEdges[i].getEdgeWeight();
            //cout << "peso depois: " << weight << endl;
        }
        //caso forme um ciclo ou o grau do vertice ultrapasse 2, retire a aresta recem adicionada
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

    cout << "peso: " << weight << endl;
    cout << endl;

    // delete [] graphEdges;
    delete [] edgeSolution;
    return aux;
}

int Graph::utilKruskalRestritivo(int grauRestricao)
{
    int isVisited[this->getOrder()];
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }
    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges); //O(e*v)
    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    EdgeInfo *edgeSolution = new EdgeInfo[listSize];
    int solutionSize = 0;
    Node *p = nullptr;
    int weight = 0;
    for (int i = 0; i < listSize; i++)
    {
        //insere a aresta no grafo auxiliar
        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());

        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil() && aux->getNode(graphEdges[i].getNodeIdSource())->getDegree() <= grauRestricao
                             && aux->getNode(graphEdges[i].getNodeIdTarget())->getDegree() <= grauRestricao)
        {
            //cout << "peso antes: " << weight << endl;
            weight += graphEdges[i].getEdgeWeight();
            //cout << "peso depois: " << weight << endl;
        }
        //caso forme um ciclo ou o grau do vertice ultrapasse 2, retire a aresta recem adicionada
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

    // delete [] graphEdges;
    delete [] edgeSolution;
    return weight;
}

Graph* Graph::kruskalAleatorio()
{
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
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
    float alpha = 0.05;
    int weight = 0;

    //cout << "nums: " << endl;
    for (int i = 0; i < listSize; i++)
    {
//       [ (3, 5, 7*), 9, 10, 12]
 //      [ 7, 5, 3, 9, 10, 12]
  //     [ x, (5, 3, 9), 10, 12]

        //insere a aresta no grafo auxiliar

        // int randomI = min(rand()%((int)(alpha*listSize)), listSize - i);
        //cout << "list-i:" << listSize - i << " alpha*list:" << (int)(alpha*listSize);
        int randomI = rand()%(min(listSize - i, (int)(alpha*listSize))) + i; // note que eu to arredondando para baixo, e.g. 3.3333 = 3;
        //cout << " "<<  randomI -i<< ", ";
        EdgeInfo auxx = graphEdges[i];
        graphEdges[i] = graphEdges[randomI];
        graphEdges[randomI] = auxx;

        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());
        // graphEdges.swap(i, randomI);


        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil())
        {
            //cout << "peso antes: " << weight << endl;
            weight += graphEdges[i].getEdgeWeight();
            //cout << "peso depois: " << weight << endl;
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

    cout << "peso: " << weight << endl;

    return aux;
}

Graph* Graph::kruskalAleatorioRestritivo(int grauRestricao, int numberIteration)
{
    // INICIO DO CODIGO SOLTA O RELOGIO
    double sum = 0;
    double add = 1;
    auto begin = std::chrono::high_resolution_clock::now();

    int iterations = 1000*1000*1000;
    for (int i=0; i<iterations; i++) {
        sum += add;
        add /= 2.0;
    }
    // FIM

    int solucaoRestritivo = utilKruskalRestritivo(grauRestricao);


	Graph *optimalGraph = nullptr;
	int bestCost = INT_MAX;
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges()/(1+(1 - (int)this->getDirected() ))); // cria um vetor com as Edges do grafo, tamanho eh dividido por dois caso o grafo seja nao direcionado
    // auto graphEdges = vector<EdgeInfo>(this->getNumberEdges()/(1)); // cria um vetor com as Edges do grafo, tamanho eh dividido por dois caso o grafo seja nao direcionado

    int isVisited[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }

    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges);
    cout << "listSize: " << listSize << endl;
    cout << "graphEdgesSize: " << graphEdges.size() << endl;

    int contador = 0;

    for(int i = 0; i < numberIteration; i++){
    	cout << "i: " << i << " ";
    	this->auxKruskalAleatorioRestritivo(grauRestricao, &bestCost, &optimalGraph, graphEdges, listSize, &contador, solucaoRestritivo);
    }

    cout << "Melhor custo de AGM: " << bestCost << endl;
    cout << "Solucao do Kruskal Restritivo: " << solucaoRestritivo << endl;
    cout << contador << " iteracoes foram melhores que Kruskal Restritivo, representando " <<
                    (double)contador/numberIteration*100 << "% do total de iteracoes" << endl;


    // INICIO DO CODIGO PAUSA O RELOGIO
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    //cout << "Result: " << sum << endl;

    cout << "Time measured: " << (elapsed.count() * 1e-9) << " seconds" << endl;
    // FIM


    return optimalGraph;
}


void Graph::auxKruskalAleatorioRestritivo(int grauRestricao, int* bestCost, Graph** optimalGraph, vector<EdgeInfo> graphEdges, int listSize, int* contador, int solKrusRes)
{
    /// auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
    /// int isVisited[this->getOrder()];
///
    /// for (int i = 0; i < this->getOrder(); i++)
    /// {
        /// isVisited[i] = -1;
    /// }
///
    /// //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    /// int listSize = this->listSortEdges(isVisited, graphEdges);

    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    // int listSize = graphEdges.size();
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    EdgeInfo *edgeSolution = new EdgeInfo[listSize];
    int solutionSize = 0;
    Node *p = nullptr;
    float alpha = 0.01;
    int weight = 0;

    //cout << "nums: " << endl;
    for (int i = 0; i < listSize; i++)
    {
        //insere a aresta no grafo auxiliar

        int randomI = rand()%(min(listSize - i, (int)(alpha*listSize))) + i; // note que eu to arredondando para baixo, e.g. 3.3333 = 3;
        //cout << " "<<  randomI -i<< ", ";
        EdgeInfo auxx = graphEdges[i];
        graphEdges[i] = graphEdges[randomI];
        graphEdges[randomI] = auxx;

        aux->insertEdge(graphEdges[i].getNodeIdSource(), graphEdges[i].getNodeIdTarget(), graphEdges[i].getEdgeWeight());

        //caso nao forme ciclo, guarde a aresta na solucao
        if(!aux->isCyclicUtil() && aux->getNode(graphEdges[i].getNodeIdSource())->getDegree() <= grauRestricao
        					 && aux->getNode(graphEdges[i].getNodeIdTarget())->getDegree() <= grauRestricao)
        {
            //cout << "peso antes: " << weight << endl;
            weight += graphEdges[i].getEdgeWeight();
            //cout << "peso depois: " << weight << endl;
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

    cout << "peso: " << weight;

    if (weight < solKrusRes)
    {
        *contador = *contador + 1;
        cout << " - melhor que KrusRes!";
    }

    if (weight < *bestCost)
    {
    	*bestCost = weight;
    	*optimalGraph = aux;
        if (*bestCost < solKrusRes)
        {
            cout << " - nova melhor solucao!!";
        }
    }

    cout << endl;

    delete [] edgeSolution;
}


bool Graph::isCyclicUtil()
{
    if (this->directed)
        return this->isCyclicDirected();
    else
        return this->isCyclic();
}


Graph* Graph::dcMST(int d, int interactions){
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
    int isVisited[this->getOrder()];
    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }
    cout << "isVisited done" << endl;
    int listSize = this->listSortEdges(isVisited, graphEdges);
    cout << "sorted edge list done" << endl;
    float* minEdgeWeight = new float;
    float* maxEdgeWeight = new float;
    /// jEdgeInfo *solutionEdges = new EdgeInfo[listSize];
    vector<EdgeInfo> solutionEdges;
    Graph* mst = dcMSTInteraction(minEdgeWeight, maxEdgeWeight, graphEdges, listSize, isVisited, solutionEdges);
    cout << "First interation done" << endl;
    float newWeight;
    float currentWeight;
    float fe;
    cout << "minEdgeWeight: " << minEdgeWeight << endl;
    cout << "maxEdgeWeight: " << maxEdgeWeight << endl;
    for(int i = 0; i < interactions; i++){
        for(int j = 0; j < solutionEdges.size(); j++){
            currentWeight = solutionEdges[j].getEdgeWeight();
            cout << currentWeight << endl;
            fe = (int)(this->getNode(graphEdges[j].getNodeIdSource())->getInDegree() > d) + (int)(this->getNode(graphEdges[j].getNodeIdTarget())->getInDegree() > d); //note que getNode eh O(n)
            // fe = 1;
            newWeight = currentWeight + fe*((currentWeight - *minEdgeWeight)/(*maxEdgeWeight - *minEdgeWeight))*(*maxEdgeWeight);
            // newWeight = 3;
            cout << currentWeight << " -> " << newWeight << " " << endl;
            solutionEdges[j].setEdgeWeight(newWeight);
            // TODO: atualizar min and max weight;
            // obs: nao acho que seja necessario

        }
        cout << "first loop done " << endl;
        mst = dcMSTInteraction(minEdgeWeight, maxEdgeWeight, graphEdges, listSize, isVisited, solutionEdges);

    }
    return mst;
}
Graph* Graph::dcMSTInteraction(float *minEdgeWeight, float *maxEdgeWeight, vector<EdgeInfo> graphEdges, int listSize, int isVisited[], vector<EdgeInfo>& solutionEdges){
    Graph *mst = new Graph(this->getOrder(), this->getDirected(),
                           this->getWeightedEdge(), this->getWeightedNode());
    Node *p = nullptr;

    int weight = 0; // TODO: if weight is negative
    // minEdgeWeight = nullptr;  // minimum edge weight in the MST
    // maxEdgeWeight = nullptr;  // maximum edge weight in the MST // Um erro, jah que o ponteiro eh perdido
    int currentWeight;
    bool maxUnset = true;
    bool minUnset = true;
    solutionEdges = vector<EdgeInfo>(); // clears solutionEdges vector;
    cout << "before for loop" << endl;
    for (int i = 0; i < listSize; i++)
    {
        cout << " i" << i << endl;
        //insere a aresta no grafo auxiliar
        mst->insertEdge(graphEdges[i].getNodeIdSource(),
                        graphEdges[i].getNodeIdTarget(),
                        graphEdges[i].getEdgeWeight());

        cout  << " jflskda" << endl;
        //caso nao forme ciclo, guarde a aresta na solucao
        if (!mst->isCyclicUtil()) {
          // solutionEdges[solutionSize] = graphEdges[i];
          solutionEdges.push_back(graphEdges[i]);
          currentWeight = graphEdges[i].getEdgeWeight();
          cout << "currentWeight: " << graphEdges[i].getEdgeWeight();
          weight += currentWeight;
          // solutionSize++;
          // updates minEdgeWeight and maxEdgeWeight if needed
          cout << "middle" << endl;
          if(!minUnset) {
              cout << *minEdgeWeight << endl;
              *minEdgeWeight = min(*minEdgeWeight, (float)(graphEdges[i].getEdgeWeight()));
          }
          else{
              cout << "entrou 1" << endl;
              *minEdgeWeight = (float)currentWeight;
              cout << "saiu 1" << endl;
              minUnset = false;
          }
          if(!maxUnset){
              cout << *maxEdgeWeight << endl;
              *maxEdgeWeight = max(*maxEdgeWeight, (float)(graphEdges[i].getEdgeWeight()));
              cout << "maxCandidate " << (float)(graphEdges[i].getEdgeWeight()) << endl;
          }
          else
              *maxEdgeWeight = (float)currentWeight;
              maxUnset = false;
        }
        // caso forme um ciclo, retire a aresta recem adicionada
        else {
            cout << "uai" << endl;
          // ponteiro aponta pra no de saida e remove a aresta
          p = mst->getNode(graphEdges[i].getNodeIdSource());
          p->removeEdge(graphEdges[i].getNodeIdTarget(), this->directed,
                        this->getNode(graphEdges[i].getNodeIdTarget()));

          // ponteiro aponta para no de chegada e remove a aresta
          p = mst->getNode(graphEdges[i].getNodeIdTarget());
          p->removeEdge(graphEdges[i].getNodeIdSource(), this->directed,
                        this->getNode(graphEdges[i].getNodeIdSource()));
        }
    }

    cout << "peso: " << weight << endl;
    cout << "maxEdgeWeight:" << *maxEdgeWeight << endl;
    cout << "minEdgeWeight:" << *minEdgeWeight << endl;
    cout << endl;

    // delete [] graphEdges;
    // delete [] edgeSolution;

    return mst;
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
    cout << "Ordem: " << this->getOrder() << endl;
    Node* p = first_node;
    while(p != nullptr){
        cout << p->getId() +1 << ": ";
        Edge* e = p->getFirstEdge();
        while(e != nullptr){
            cout << "->" << e->getTargetId()+1 << " ";
            e = e->getNextEdge();
        }
        cout << "\t GRAU: " << p->getDegree() << endl;
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

void Graph::escreverEmArquivoTeste(ofstream& output_file){

    output_file << "graph GrafoOrdem" << this->getOrder() << " {" << endl;
    Node *p = this->getFirstNode();
    while (p != nullptr)
    {
        output_file << p->getId();
        Edge* e = p->getFirstEdge();
        while (e != nullptr)
        {
            output_file << " -- " << e->getTargetId();
            e = e->getNextEdge();
        }
        output_file << endl;
        p = p->getNextNode();
    }
    output_file << endl;
    output_file << "}";


    return;
}

Graph* Graph::kruskal2(){
    auto begin = std::chrono::high_resolution_clock::now();
    // auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
    // int isVisited[this->getOrder()];
//
    // for (int i = 0; i < this->getOrder(); i++)
    // {
        // isVisited[i] = -1;
    // }
    // cout << "isVisited: ";
    // for (int i = 0; i < this->getOrder(); i++)
        // cout << isVisited[i] << ", ";
//
    // //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    // int listSize = this->listSortEdges(isVisited, graphEdges);
    // //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    // //se o grafo se tornou ciclo ou não
    // Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    // auto edgeSolution = vector<EdgeInfo>(listSize);
    // int solutionSize = 0;
    // Node *p = nullptr;
//
    // int weight = 0;
    DisjointSetForest forest(this->getOrder());
    // // [ -1, -1, -1, ..., -1]
    // bool solution = false;
    int sourceParent, targetParent;
    EdgeInfo edge;
    // cout << "ListSize: " << listSize << endl;
    // cout << "graphEdgesSize: " << graphEdges.size() << endl;
    // cout << "first edge: " << graphEdges[0].getNodeIdSource() << endl;
    auto graphEdges = vector<EdgeInfo>(this->getNumberEdges());
    auto remainingEdges = vector<EdgeInfo>();
    int isVisited[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = -1;
    }
    //coloca todas as arestas no vetor de EdgeInfo e ordena por peso;
    int listSize = this->listSortEdges(isVisited, graphEdges); //O(e*v)
    //cria um grafo auxiliar que introduz as arestas mais baratas e testa uma a uma no novo grafo que contem todos os nós do grafo original
    //se o grafo se tornou ciclo ou não
    Graph *aux = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    EdgeInfo *edgeSolution = new EdgeInfo[listSize];
    int solutionSize = 0;
    Node *p = nullptr;
    int weight = 0;
    int i;
    for(i =0; i < listSize; i++){
        edge = graphEdges[i];
        // cout << i << ": " << edge.getNodeIdSource() << "->" << edge.getNodeIdTarget() << " (" << edge.getEdgeWeight() << ")" << endl;
        sourceParent = forest.find(edge.getNodeIdSource()); // -1
        targetParent = forest.find(edge.getNodeIdTarget()); // -1
        if(sourceParent == -1 || targetParent == -1){
            cout << "out of bounds!!!" << endl;
            return aux;
        }
        if(sourceParent >= this->getOrder() || targetParent >= this->getOrder()){
            cout << "error" << endl;
            return aux;
        }
        // cout << "i: "
            // << edge.getNodeIdSource() << " (pai = "<< sourceParent << ") " << ", "
            // << edge.getNodeIdTarget() << " (pai = "<< targetParent << ") " << ", " << endl;

        if(sourceParent != targetParent){
            forest.setUnion(sourceParent, targetParent);
            // edgeSolution.push_back(edge);
            weight += edge.getEdgeWeight();
            aux->insertEdge(edge.getNodeIdSource(), edge.getNodeIdTarget(), edge.getEdgeWeight());
        }
        else {
            remainingEdges.push_back(edge);
            continue;
        }

    }
    int q = 5;
    auto edgesLeavingQtree =forest.getEdgesLeavingSubTree(q, remainingEdges);
    forest.print();
    for(auto edge : edgesLeavingQtree){
        cout << edge.getNodeIdSource() << "->" << edge.getNodeIdTarget() << ",  ";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "\n\n\nduracao: " << (elapsed.count() * 1e-9)<< endl;
    cout << "peso: " << weight << endl;
    // cout << "maxEdgeWeight:" << *maxEdgeWeight << endl;
    // cout << "minEdgeWeight:" << *minEdgeWeight << endl;
    // aux->print();
    return aux;


}
