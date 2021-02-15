#ifndef CHAINEDQUEUE_H_INCLUDED
#define CHAINEDQUEUE_H_INCLUDED
#include "QNode.h"

class ChainedQueue
{
private:
	QNode *first; 
	QNode *last;
	int size;

public:
	ChainedQueue();
	~ChainedQueue();
	int getFirst(); 
	void queueUp(int val);
	int queueDown(); 
	bool isEmpty();
	int getSize();
	void print();
};

#endif