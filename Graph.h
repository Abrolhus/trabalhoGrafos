/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>

#define INF 10000.0f

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id);
        void insertEdgePreguicoso(int id, int target_id, float weight);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);

        //methods phase1
        void topologicalSorting();
        void breadthFirstSearch(int idFirstNode);
        Graph* getVertexInduced(int* listIdNodes);
        Graph* agmKuskal();
        Graph* agmPrim();
        float floydMarshall(int idSource, int idTarget);
        float dijkstra(int idSource, int idTarget);

        //methods phase1
        float greed();
        void escreverEmArquivo(ofstream& output_file);
        float greedRandom();
        float greedRactiveRandom();
        void print();

        bool auxGetVertexInduced (int id, int* listIdNodes);
        bool isCyclic();
        bool auxIsCyclic(int nodeId, bool isVisited[], int parentId);
        bool isCyclicDirected();
        bool auxIsCyclicDirected(int nodeId, bool isVisited[], bool *isContainedRecusirve);
    private:
        //Auxiliar methods

};

#endif // GRAPH_H_INCLUDED
