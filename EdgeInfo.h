#ifndef EDGEINFO_H_INCLUDED
#define EDGEINFO_H_INCLUDED
#define MAX 1000000;

class EdgeInfo
{

public:
	EdgeInfo() { nodeIdSource = MAX
				 nodeIdTarget = MAX
				 edgeWeight = MAX; };
	EdgeInfo(int source, int target, float weight){
		nodeIdSource = source;
		nodeIdTarget = target;
		edgeWeight = weight;
	}
	~EdgeInfo() { };

	//getters
	int getNodeIdSource() { return nodeIdSource; };
	int getNodeIdTarget() { return nodeIdTarget; };
	float getEdgeWeight() { return edgeWeight; };

	//setters
	void setNodeIdSource(int id) { nodeIdSource = id; };
	void setNodeIdTarget(int id) { nodeIdTarget = id; };
	void setEdgeWeight(float weight) { edgeWeight = weight; };

private:
    int nodeIdSource;
    int nodeIdTarget;
    float edgeWeight;
};

#endif
