#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
	Node() { in = 0; }
	int in;
	vector<int> adj;
};

int main()
{
	int numNodes, numAdj, neighbor;
	cin >> numNodes;
	vector<Node> nodeList(numNodes);
	for (int i = 0; i < numNodes; i++) {
		cin >> numAdj;
		for (int j = 0; j < numAdj; j++) {
			cin >> neighbor;
			neighbor--;
			nodeList[i].adj.push_back(neighbor);
			nodeList[neighbor].in++;
		}
	}
	queue<int> srclist;
	for (int i = 0; i < numNodes; i++)
		if (nodeList[i].in == 0)
			srclist.push(i);
	vector<int> sorted;
	while (!srclist.empty()) {
		int cur = srclist.front(); srclist.pop();
		sorted.push_back(cur);
		for (int i = 0; i < nodeList[cur].adj.size(); i++) {
			int next = nodeList[cur].adj[i];
			if (--nodeList[next].in == 0)
				srclist.push(next);
		}
	}
	if (numNodes != sorted.size()) {
		cout << "Not a DAG!\n";
	}
	else {
		for (int i = 0; i < numNodes; i++) {
			cout << sorted[i] << endl;
		}
	}
}