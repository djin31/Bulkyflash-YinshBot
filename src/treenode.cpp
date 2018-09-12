#include "treenode.h"

Treenode::Treenode(){
	move_description="";
	value = 0;
}

void Treenode::generate_children(){
	vector<pair<Board*,string>> child_config = board->get_valid_moves();
	children = vector<Treenode*>(child_config.size());
	// Treenode* r = new Treenode();
	// r->board = child_config[0].first;
	// r->board->printBoard();
	for (int i=0;i<child_config.size();i++)
	{
		children[i]=new Treenode();
		children[i]->board = child_config[i].first;
		children[i]->move_description = child_config[i].second;
	}
}