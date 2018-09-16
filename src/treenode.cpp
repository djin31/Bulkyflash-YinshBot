#include "treenode.h"

Treenode::Treenode(){
	move_description="";
	value = 0;
}

Treenode::~Treenode(){
	delete_children();
	delete board;
}

void Treenode::generate_children(){
	vector<pair<Board*,string>> child_config = board->get_valid_moves();
	children = vector<Treenode*>(child_config.size());
	
	for (int i=0;i<child_config.size();i++)
	{
		children[i]=new Treenode();
		children[i]->board = child_config[i].first;
		children[i]->move_description = child_config[i].second;
	}
}

void Treenode::delete_children(){
	for (int i=0; i < children.size(); i++)
	{
		delete children[i];
	}
	children.clear();
}

Treenode* Treenode::copyNode(){
	Treenode *newNode;
	newNode->board = this->board->copy_board();
	newNode->move_description = this->move_description;
	return newNode;
}