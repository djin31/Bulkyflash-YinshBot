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
		//vector<string> children_strings;

		Treenode();
		~Treenode();

		void generate_children();
		//void generate_children_strings();
		void delete_children();
		Treenode* copyNode();
};
#endif