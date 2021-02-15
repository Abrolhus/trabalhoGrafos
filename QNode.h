#ifndef QNODE_H_INCLUDED
#define QNODE_H_INCLUDED

class QNode
{

public:
	QNode() { };
	~QNode() { };
	int getInfo() { return info; };
	QNode* getNextNode() { return next; };
	void setInfo(int value) { info = value; };
	void setNextNode(QNode *p) { next = p; };

private:
	int info;
	QNode *next;
};

#endif