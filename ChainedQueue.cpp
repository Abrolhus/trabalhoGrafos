#include <iostream>
#include "QNode.h"
#include "ChainedQueue.h"
using namespace std;

ChainedQueue::ChainedQueue()
{
    first = nullptr;
    last = nullptr;
    size = 0;
}

ChainedQueue::~ChainedQueue()
{
    cout << "Destruindo objeto ChainedQueue..." << endl;

    QNode *p = first;
    QNode *aux = nullptr;

    while (p != nullptr)
    {
        aux = p;
        p = p->getNextNode();
        delete aux;
    }

}

bool ChainedQueue::isEmpty()
{
    if (first == nullptr)
        return true;
    else
        return false;
}

int ChainedQueue::getFirst()
{
    if (!isEmpty())
    {
        return first->getInfo();
    }
    else
    {
        cout << endl << "ERROR: Empty list" << endl;
        exit(1);
    }
}

void ChainedQueue::queueUp(int info)
{
    //cria novo node e inicializa ele de forma correta
    QNode *p = new QNode();
    p->setInfo(info);
    p->setNextNode(nullptr);
    size++;

    //testa para o caso de fila vazia
    if (!isEmpty())
    {
        //caso nao esteja vazia, aloca no fim
        last->setNextNode(p);
        last = p;
    }
    else
    {
        //caso seja a insercao do primeiro node
        first = p;
        last = p;
    }
}

int ChainedQueue::queueDown()
{
    QNode *p = first;
    int info;

    if (!isEmpty())
    {
        info = first->getInfo();
        first = p->getNextNode();
        delete p;
        size--;
        return info;
    }
    else
    {
        cout << endl << "ERROR: Empty list" << endl;
        exit(1);
    }
}

int ChainedQueue::getSize()
{
    return size;
}

void ChainedQueue::print()
{
    QNode *p = first;

    cout << endl << "Queue: ";

    while (p != nullptr)
    {
        cout << p->getInfo() << " ";
        p = p->getNextNode();
    }
}