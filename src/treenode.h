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
		~Treenode();

		void generate_children();
		void delete_children();
		Treenode* copyNode();
};
#endif