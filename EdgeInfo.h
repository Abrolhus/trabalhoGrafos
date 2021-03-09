#ifndef EDGEINFO_H_INCLUDED
#define EDGEINFO_H_INCLUDED
#define MAX 10000;

class EdgeInfo
{

public:
	EdgeInfo() { nodeIdSource = MAX
				 nodeIdTarget = MAX
				 edgeWeight = MAX; };
	EdgeInfo(int source, int target, int weight){
		nodeIdSource = source;
		nodeIdTarget = target;
		edgeWeight = weight;
	}
	~EdgeInfo() { };

	//getters
	int getNodeIdSource() { return nodeIdSource; };
	int getNodeIdTarget() { return nodeIdTarget; };
	int getEdgeWeight() { return edgeWeight; };

	//setters
	void setNodeIdSource(int id) { nodeIdSource = id; };
	void setNodeIdTarget(int id) { nodeIdTarget = id; };
	void setEdgeWeight(int weight) { edgeWeight = weight; };

private:
    int nodeIdSource;
    int nodeIdTarget;
    int edgeWeight;
};

#endif
