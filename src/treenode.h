#ifndef NODE_H
#define NODE_H

#include "board.h"
using namespace std;

class Treenode{
	public:
		Board* board;
		string move_description;
		double value;
		vector<Treenode*> children;

		Treenode();

		void generate_children();
};
#endif