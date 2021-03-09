/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "EdgeInfo.h"
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>
#include <vector>

#define INF 1000000.0f

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
        Graph* dcMSTInteraction(float* minEdgeWeight, float* maxEdgeWeight, std::vector<EdgeInfo> edges, int listSize, int isVisited[], std::vector<EdgeInfo>& solutionEdges);

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
<<<<<<< Updated upstream
        void breadthFirstSearch(int idFirstNode, int ordem[]);
=======
        void breadthFirstSearch(int idFirstNode);
        void auxBreadthFirstSearch(int idFirstNode, int ordem[]);
>>>>>>> Stashed changes
        Graph* dcMST(int d, int interactions);
        Graph* getVertexInduced(int* listIdNodes);
        Graph* prim();
        float floydMarshall(int idSource, int idTarget);
        float dijkstra(int idSource, int idTarget);
        void connectionsFloyd();
        bool isConnected();

        //Kruskals
        Graph* kruskal();
        Graph* kruskalAleatorio();
        Graph* kruskalRestritivo(int grauRestricao);
        int utilKruskalRestritivo(int grauRestricao);
        void auxKruskalAleatorioRestritivo(int grauRestricao, int* bestCost, Graph** optimalGraph, std::vector<EdgeInfo> graphEdges, int listSize, int* contador, int solKrusRes);
        Graph* kruskalAleatorioRestritivo(int grauRestricao, int numberIteration);
        Graph* kruskal2();
        Graph* kruskal3(int grauRestricao);

        //methods phase1
        float greed();
        void escreverEmArquivo(ofstream& output_file);
        void escreverEmArquivoTeste(ofstream& output_file);
        float greedRandom();
        float greedRactiveRandom();
        void print();
        bool isConnected();

        bool checkContainsId(int id, int nodeList[], int listLength);
    private:
        //Auxiliar methods
        int listSortEdges(int isVisited[], vector<EdgeInfo>& graphEdges);
        int cheapestNeighbor(int neighborPath[], int visitedNodes[]);
        void updateNeighborPath(int id, int neighborPath[]);
        bool isCyclic();
        bool auxGetVertexInduced (int id, int* listIdNodes);
        bool auxIsCyclic(int nodeId, bool isVisited[], int parentId);
        bool isCyclicDirected();
        bool auxIsCyclicDirected(int nodeId, bool isVisited[], bool *isContainedRecusirve);
        bool isCyclicUtil();
        vector<bool> getSonsOf(int father, int idFirstNode);
        int getFatherOf(int node, int root);



};

#endif // GRAPH_H_INCLUDED
