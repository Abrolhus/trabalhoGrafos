#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include "ChainedQueue.h"
#include "Graph.h"
#include "Node.h"

using namespace std;

Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    cout << "Criando grafo..." << endl;
    cout << "Ordem: " << graph->getOrder() << endl;
    cout << "Direcionado? " << graph->getDirected() << endl;
    cout << "Arestas com peso? " << graph->getWeightedEdge() << endl;
    cout << "Vertices com peso? " << graph->getWeightedNode() << endl;

    int cont = 1;

    //Leitura de arquivo

    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){

        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdgePreguicoso(idNodeSource, idNodeTarget, 0);

        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {
            //cout << "Inserindo aresta #" << cont << ": " << idNodeSource << " -> " << idNodeTarget << ". Peso: " << edgeWeight << endl;
            graph->insertEdgePreguicoso(idNodeSource, idNodeTarget, edgeWeight);
            cont++;

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdgePreguicoso(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdgePreguicoso(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }

    return graph;
}



Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    // input_file >> order >> numEdges;
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);
    graph->insertEdge(0,1,3);
    graph->insertEdge(0,4,6);
    graph->insertEdge(1,4,2);
    graph->insertEdge(3,4,1);
    graph->insertEdge(4,2,7);

    // Leitura de arquivo
    while(input_file >> idNodeSource >> idNodeTarget) {

        // graph->insertEdge(idNodeSource, idNodeTarget, 0);
        // cout << "arquivo: " << idNodeSource<< "->" << idNodeTarget<< endl;
        graph->insertEdge(idNodeSource-1, idNodeTarget-1, 0);
        if(!graph->getDirected())
            graph->insertEdge(idNodeTarget-1, idNodeSource-1, 0);
    }

    return graph;
}
Graph* leituraInstancia_PARA_TESTES(ifstream& input_file, int directed, int weightedEdge, int weightedNode){
    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    // input_file >> order >> numEdges;
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);
    graph->insertEdge(0,1,3);
    graph->insertEdge(0,4,6);
    graph->insertEdge(1,4,2);
    graph->insertEdge(3,4,1);
    graph->insertEdge(4,2,7);

    // Leitura de arquivo
//    while(input_file >> idNodeSource >> idNodeTarget) {
//
//        // graph->insertEdge(idNodeSource, idNodeTarget, 0);
//        // cout << "arquivo: " << idNodeSource<< "->" << idNodeTarget<< endl;
//        graph->insertEdge(idNodeSource-1, idNodeTarget-1, 0);
//        if(!graph->getDirected())
//            graph->insertEdge(idNodeTarget-1, idNodeSource-1, 0);
//    }

    return graph;
}


int menu(){

    int selecao;

    cout << endl << endl;
    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Subgrafo induzido por conjunto de vértices" << endl;
    cout << "[2] Caminho Mínimo entre dois vértices - Dijkstra" << endl;
    cout << "[3] Caminho Mínimo entre dois vértices - Floyd" << endl;
    cout << "[4] Árvore Geradora Mínima de Prim" << endl;
    cout << "[5] Árvore Geradora Mínima de Kruskal" << endl;
    cout << "[6] Imprimir caminhamento em largura" << endl;
    cout << "[7] Imprimir ordenacao topológica" << endl;
    cout << "[8] Algoritmo Guloso" << endl;
    cout << "[9] Algoritmo Guloso Randomizado " << endl;
    cout << "[10] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[11] Conexidade" << endl;
    cout << "[12] Escreve grafo em arquivo" << endl;
    cout << "[13] Kruskal com restricao de grau" << endl;
    cout << "[14] Kruskal aleatorio com restricao de grau" << endl;
    cout << "[15] Kruskal aleatorio" << endl;
    cout << "[16] Kruskal India" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    // MOSTRANDO O GRAFO AQUI
    // cout << endl << "Grafo vigente: " << endl;
    // graph->print();
    cout << endl;

    switch (selecao)
    {

        //Subgrafo induzido por um conjunto de vértices X;
        case 1:{

            int tamNosLista;

            cout << "Quantos nos deseja?" << endl;
            cin >> tamNosLista;
            tamNosLista++;

            cout << endl << "Quais nos devem induzir o grafo?" << endl;
            int nosLista[tamNosLista];
            int proxNo;

            for (int i = 0; i < tamNosLista - 1; i++)
            {
                cin >> proxNo;
                nosLista[i] = proxNo - 1;
            }

            nosLista[tamNosLista - 1] = -1;

            cout << endl << endl;
            cout << "Nos escolhidos: ";
            for (int i = 0; i < tamNosLista - 1; i++)
            {
                cout << nosLista[i] + 1 << " ";
            }

            cout << endl;
            Graph* novoGrafo = graph->getVertexInduced(nosLista);
            novoGrafo->print();

            break;
        }
            //Caminho mínimo entre dois vértices usando Dijkstra;
        case 2:
        {
            cout << " dijkstra: " << graph->dijkstra(0, 6) << endl;

            break;
        }
        //Caminho mínimo entre dois vértices usando Floyd;
        case 3:{

            cout << "floydMarshall: " <<  graph->floydMarshall(0, 6) << endl;

            break;
        }
        //AGM - Kruscal;
        case 5:{

            Graph* kruskal = graph->kruskal();
            // cout << endl << "Arvore Geradora Minima usando Kruskal: " << endl;
            // kruskal->print();
            // cout << endl;
            // cout << endl;
            // cout << endl;

            // vector<Graph*> kruskRands;
            // for(int i = 0; i < 200; i++){
            //     kruskRands.push_back(graph->kruskalAleatorio(2));
            //     // kruskRand->print();
            // }

            // Graph* kruskal = graph->auxKruskalAleatorio(3, 200);
            // kruskal->print();


            break;

        }
        //AGM Prim;
        case 4:{

            Graph* prim = graph->prim();
            cout << endl << "Arvore Geradora Minima usando Prim: " << endl;
            prim->print();

            break;
        }
        //Busca em largura;
        case 6:{

            int primeiroNo;
            cout << endl << "A partir de qual no deseja fazer a busca em largura? " << endl;
            cin >> primeiroNo;

            int ordemLargura[graph->getOrder()];
            graph->breadthFirstSearch(primeiroNo);

            int auxLargura[graph->getOrder()];
            cout << "Ordem de descoberta: "<< endl;
            for (int i = 0; i < graph->getOrder(); i++)
            {
                auxLargura[ordemLargura[i] - 1] = i;
                cout << auxLargura[i]  << " ";
            }

            cout << endl << "É conexo? " << graph->isConnected();

            // graph->breadthFirstSearch(primeiroNo);
            // cout << endl;
            // cout << "Eh conexo? " << graph->isConnected() << endl;


            break;
        }
            //Ordenação Topologica;
        case 7:{

            graph->topologicalSorting();

            break;
        }
        case 8:{
            cout << "dcMST: " << endl;
            auto* dcMST = graph->dcMST(2, 10);
            cout << "dcMST: " << endl;
            dcMST->print();

            break;
       }
       case 11:{

            graph->connectionsFloyd();


            break;
       }
       case 12:{

            graph->escreverEmArquivoTeste(output_file);
            break;
       }

       case 13:{
            int grauRestricao;
            cout << "Deseja restringir a que grau? " << endl;
            cin >> grauRestricao;
            Graph* kruskalRestritivo = graph->kruskalRestritivo(grauRestricao);
            kruskalRestritivo->print();
            break;
       }

       case 14:{
            int grauRestritivo;
            int numeroIteracoes;

            graph->print();

            cout << "Deseja restringir a que grau? " << endl;
            cin >> grauRestritivo;

            cout << "Quantas iteracoes deseja?" << endl;
            cin >> numeroIteracoes;

            Graph* kruskalRes = graph->kruskalAleatorioRestritivo(grauRestritivo, numeroIteracoes);
            cout << "Melhor solucao eh viavel? " << kruskalRes->isConnected() << endl;
            cout << "Busca em largura: " << endl;
            kruskalRes->breadthFirstSearch(0);
            break;

       }

       case 15:{

            Graph* kruskalAle = graph->kruskalAleatorio();
            //kruskalAle->print();
            break;

       }
       case 16:{

            // Graph* krusk2 = graph->kruskal2();
            Graph* krusk3 = graph->kruskal3(3);
            krusk3->print();
            break;

       }

        case 0:{

            break;
        }
        default:{
            cout << " Error!!! invalid option!!" << endl;
        }
    }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        // system("clear");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

    }

    return 0;
}



int main(int argc, char const *argv[]) {

    srand(time(NULL));
    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);



    Graph* graph;

    if(input_file.is_open()){

        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

    //graph->escreverEmArquivoTeste(output_file);

    /////graph->print();
    /////graph->getNode(4)->removeEdge(0, 0, graph->getNode(0));
    /////cout << graph->getNode(0)->getId() << endl;
    ///////graph->getNode(0)->removeEdge(4, 0, graph->getNode(4));
    /////graph->print();
    // graph->getFirstNode()->removeEdge(

    mainMenu(output_file, graph);
    // cout << "teste 123" << endl;
    // graph->print();
    // //mainMenu(output_file, graph);


    //cout << "Grafo: " << endl;
    //graph->print();

    //cout << endl << endl;

    //graph->prim();
    //graph->kruskal();

    // // --- teste da letra a) ---
    // cout << endl;
    // cout << "Testando letra a)" << endl;
    // cout << "Subgrafo Gerado pelos vértices 1, 2, 4, 6, 8, 9:" << endl;
    // int listnos[7] = {0, 1, 3, 5, 7, 8, -1};
    // Graph* novoGrafo = graph->getVertexInduced(listnos);
    // novoGrafo->print();
    // ------------------------


    // // --- teste da letra f) ---
    // cout << endl;
    // cout << "Teste para Busca em Largura (letra f)" << endl;
    // graph->breadthFirstSearch(1);
    // cout << endl;


    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

