#include <algorithm>
#include <math.h>
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

            cout << "Inserindo aresta: " << idNodeSource << "->" << idNodeTarget << endl;
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

Graph* leituraEUC_2D(ifstream& input_file, int directed, int weightedEdge, int weightedNode){
    // int n = std::count(std::istreambuf_iterator<char>(input_file),
                 // std::istreambuf_iterator<char>(), '\n');
    // cout << "num de linhas: " <<  n << endl;;
    double n, x, y;
    std::vector<std::pair<double, double>> valores;
    string line;
    for(int i = 0; i<6; i++)
        std::getline(input_file, line);
    while(input_file >> n >> x >> y ){
        valores.push_back(std::pair<double,double>(x, y));
    }
    // for(auto valor : valores){
    //     cout << valor.first << ", " << valor.second << endl;
    // }
    //cout << valores.size() << endl;
    Graph* graph = new Graph(valores.size(), directed, weightedEdge, weightedNode);
    for(int i = 0; i < valores.size(); i++){
        for(int j = i+1; j < valores.size(); j++){
            double x1 = valores.at(i).first;
            double x2 = valores.at(j).first;
            double y1 = valores.at(i).second;
            double y2 = valores.at(j).second;
            // cout << sqrt(hypot(x1 - x2, y1 - y2));
            graph->insertEdge(i, j, (float)round(hypot(x1 - x2, y1 - y2)));
        }
    }
    //cout << graph->getNumberEdges() << endl;
    //graph->print();
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
    cout << "[9] Algoritmo Guloso Randomizado" << endl;
    cout << "[10] Conexidade" << endl;
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

            cout << "Subgrafo induzido por conjunto de vertices selecionado" << endl << endl;

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
                nosLista[i] = proxNo;
            }

            nosLista[tamNosLista - 1] = -1;

            cout << endl << endl;
            cout << "Nos escolhidos: ";
            for (int i = 0; i < tamNosLista - 1; i++)
            {
                cout << nosLista[i]  << " ";
            }

            cout << endl;
            Graph* novoGrafo = graph->getVertexInduced(nosLista);
            novoGrafo->print();

            break;
        }
            //Caminho mínimo entre dois vértices usando Dijkstra;
        case 2:
        {

            cout << "Distancia minima entre dois vertices usando algoritimo de Dijkstra selecionado" << endl << endl;

            int vert1;
            cout << "Indique o primeiro vertice: " << endl;
            cin >> vert1;

            int vert2;
            cout << "Indique o segundo vertice: " << endl;
            cin >> vert2;

            cout << endl;

            cout << "Distancia entre" << vert1 << " e " << vert2 << " :" << graph->dijkstra(vert1, vert2) << endl;

            break;
        }
        //Caminho mínimo entre dois vértices usando Floyd;
        case 3:{

            cout << "Distancia minima entre dois vertices usando algoritimo de Floyd-Marshall selecionado" << endl << endl;

            int vertex1;
            cout << "Indique o primeiro vertice: " << endl;
            cin >> vertex1;

            int vertex2;
            cout << "Indique o segundo vertice: " << endl;
            cin >> vertex2;

            char querMatriz;
            cout << "Deseja imprimir toda a matriz? (y/n)" << endl;
            cin >> querMatriz;

            cout << endl;

            if (querMatriz == 'y')
            {
                //cout << "Matriz completa e distancia entre os vertices selecionados: " << endl;
                cout << graph->floydMarshall(vertex1, vertex2, true);
            }
            else
            {
                //cout << "Distancia entre os vertices selecionados: " << endl;
                cout << graph->floydMarshall(vertex1, vertex2, false);
            }

            break;
        }
        //AGM - Prim;
        case 4:{

            cout << "AGM usando algoritimo de Prim selecionado" << endl << endl;

            Graph* prim = graph->prim();

            break;

        }
        //AGM Kruskal;
        case 5:{

            cout << "AGM usando algoritimo de Kruskal selecionado" << endl << endl;

            Graph* kruskalComum = graph->kruskal2();

            break;
        }
        //Busca em largura;
        case 6:{

            cout << "Caminhamento em largura selecionado" << endl << endl;

            int primeiroNo;
            cout << endl << "A partir de qual no deseja fazer a busca em largura? " << endl;
            cin >> primeiroNo;

            int ordemLargura[graph->getOrder()];
            graph->breadthFirstSearch(primeiroNo, ordemLargura);

            int auxLargura[graph->getOrder()];

            cout << "Ordem de descoberta: "<< endl;
            for (int i = 0; i < graph->getOrder(); i++)
            {
                auxLargura[ordemLargura[i] - 1] = i;
                //cout << ordemLargura[i] - 1 << " ";
            }
            for (int i = 0; i < graph->getOrder(); i++)
            {
                cout << auxLargura[i] << " ";
            }

            cout << endl << "É conexo? " << boolalpha << graph->isConnected();

            break;
        }
            //Ordenação Topologica;
        case 7:{

            cout << "Ordenação Topologica selecionado" << endl << endl;

            if (!graph->getDirected())
            {
                cout << "Solucao inviavel. Grafo nao eh direcionado!" << endl;
                break;
            }

            graph->topologicalSorting();

            break;
        }
        case 8:{
            // cout << "dcMST: " << endl;
            // auto* dcMST = graph->dcMST(2, 10);
            // cout << "dcMST: " << endl;
            // dcMST->print();

            cout << "Algoritmo Guloso Restritivo selecionado" << endl << endl;

            if (!graph->isConnected())
            {
                cout << "Solucao inviavel. Grafo nao eh conexo!" << endl;
                break;
            }

            int grauRes;
            cout << "Deseja restringir a que grau?" << endl;
            cin >> grauRes;

            cout << "Resultado: " << endl << endl;
            Graph *gulosoRes = graph->algoritmoGulosoRestritivo(grauRes);

            break;
        }
        case 9:{

            cout << "Algoritmo Guloso Randomizado Restritivo selecionado" << endl << endl;

            if (!graph->isConnected())
            {
                cout << "Solucao inviavel. Grafo nao eh conexo!" << endl;
                break;
            }

            int grauRestricao;
            cout << "Deseja restringir a que grau?" << endl;
            cin >> grauRestricao;

            float alpha;
            cout << "Deseja utilizar que alpha na randomizacao?" << endl;
            cin >> alpha;

            int numeroIteracoes;
            cout << "Deseja iterar quantas vezes?" << endl;
            cin >> numeroIteracoes;

            cout << "Resultado: " << endl << endl;
            Graph *gulosoResAle = graph->algoritmoGulosoAleatorioRestritivo(grauRestricao, numeroIteracoes, alpha);

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

void realizaTesteParte2(ofstream& output_file)
{
    output_file << "LISTA DE TESTE PARA 10 INSTANCIAS " << endl;
    output_file << "Valores de alpha testados: 0.50, 0.30, 0.15, 0.10, 0.05, 0.01" << endl;
    output_file << "Instancias testadas: pr136, pr264, pr439, u724, rat783, u1060, vm1084, fl1400, pr2392, pcb3038" << endl;
    output_file << "Podem ser encontradas em: http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/index.html" << endl;
    output_file << endl;
    output_file << " -- Metodologia -- " << endl;
    output_file << "Para cada instancia sera construido o algoritimo guloso mais 10 iteracoes do algoritimo guloso randomizado para cada valor de alpha" << endl;
    output_file << endl;
    output_file << " -- INICIO DA EXPERIMENTACAO --" << endl;
    output_file << endl << endl;

    Graph* kruskalRes = nullptr;


    // ------------- pr136 -------------
    ifstream grafoText1;
    grafoText1.open("pr136.txt");

    Graph* grafo = leituraEUC_2D(grafoText1, 0, 1, 0);
    cout << "pr136 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "pr136 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "pr136 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "pr136 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "pr136 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "pr136 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "pr136 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "pr136 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- pr264 -------------
    ifstream grafoText2;
    grafoText2.open("pr264.txt");

    grafo = leituraEUC_2D(grafoText2, 0, 1, 0);
    cout << "pr264 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "pr264 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "pr264 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "pr264 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "pr264 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "pr264 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "pr264 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "pr264 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- pr439 -------------
    ifstream grafoText3;
    grafoText3.open("pr439.txt");

    grafo = leituraEUC_2D(grafoText3, 0, 1, 0);
    cout << "pr439 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "pr439 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "pr439 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "pr439 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "pr439 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "pr439 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "pr439 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "pr439 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- u724 -------------
    ifstream grafoText4;
    grafoText4.open("u724.txt");

    grafo = leituraEUC_2D(grafoText4, 0, 1, 0);
    cout << "u724 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "u724 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "u724 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "u724 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "u724 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "u724 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "u724 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "u724 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- rat783 -------------
    ifstream grafoText5;
    grafoText5.open("rat783.txt");

    grafo = leituraEUC_2D(grafoText5, 0, 1, 0);
    cout << "rat783 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "rat783 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "rat783 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "rat783 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "rat783 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "rat783 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "rat783 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "rat783 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- u1060 -------------
    ifstream grafoText6;
    grafoText6.open("u1060.txt");

    grafo = leituraEUC_2D(grafoText6, 0, 1, 0);
    cout << "u1060 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "u1060 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "u1060 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "u1060 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "u1060 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "u1060 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "u1060 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "u1060 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- vm1084 -------------
    ifstream grafoText7;
    grafoText7.open("vm1084.txt");

    grafo = leituraEUC_2D(grafoText7, 0, 1, 0);
    cout << "vm1084 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "vm1084 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "vm1084 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "vm1084 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "vm1084 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "vm1084 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "vm1084 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "vm1084 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- fl1400 -------------
    ifstream grafoText8;
    grafoText8.open("fl1400.txt");

    grafo = leituraEUC_2D(grafoText8, 0, 1, 0);
    cout << "fl1400 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "fl1400 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "fl1400 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "fl1400 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "fl1400 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "fl1400 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "fl1400 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "fl1400 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- pr2392 -------------
    ifstream grafoText9;
    grafoText9.open("pr2392.txt");

    grafo = leituraEUC_2D(grafoText9, 0, 1, 0);
    cout << "pr2392 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "pr2392 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "pr2392 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "pr2392 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "pr2392 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "pr2392 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "pr2392 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "pr2392 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);

    // ------------- vm1748 -------------
    ifstream grafoText10;
    grafoText10.open("vm1748.txt");

    grafo = leituraEUC_2D(grafoText10, 0, 1, 0);
    cout << "vm1748 lido!" << endl;

    //kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 1, 0.01, output_file);

    cout << "vm1748 nao aleatorio rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaRestritivo(3, output_file);

    cout << "vm1748 alpha 0.5 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.5, output_file);

    cout << "vm1748 alpha 0.3 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.3, output_file);

    cout << "vm1748 alpha 0.15 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.15, output_file);

    cout << "vm1748 alpha 0.1 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.1, output_file);

    cout << "vm1748 alpha 0.05 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.05, output_file);

    cout << "vm1748 alpha 0.01 rodando..." << endl;
    kruskalRes = grafo->kruskalIndiaAleatorioRestritivo(3, 10, 0.01, output_file);
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
        //raph = leituraEUC_2D(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
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
