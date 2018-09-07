#ifndef NODE_H
#define NODE_H

class Treenode{
	public:
		Board* board;
		double value;
		double alpha;
		double beta;
		vector<Board*> children;

}
#endif