#include "treenode.h"

bool node_compare(Treenode* a, Treenode* b)
{
	return (a->value < b->value);
}

Treenode::Treenode(){}

void Treenode::generate_children(){
	vector<pair<Board*,string>> child_config = board->get_valid_moves();
	children = new vector<Treenode*>(child_config.size());
	for (int i=0;i<child_config.size();i++)
	{
		children[i]->board = child_config[i].first;
		children[i]->move_description = child_config[i].second;
	}
}