#include "treenode.h"

Treenode::Treenode(){}

void Treenode::generate_children(int player_id){
	vector<pair<Board*,string>> child_config = board->get_valid_moves(player_id);
	children = vector<Treenode*>(child_config.size());
	for (int i=0;i<child_config.size();i++)
	{
		children[i]->board = child_config[i].first;
		children[i]->move_description = child_config[i].second;
	}
}