#include "bot.h"

Bot::Bot(int player_id){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	this->player_id = player_id;
}

bool node_compare(Treenode* a, Treenode* b)
{
	return (a->value < b->value);
}

void Bot::minVal(Treenode* node, double alpha, double beta, int depth_left){
	if (depth_left==0||node->board->check_terminal())
		node->value = node->board->eval_func();
	for (Treenode* child: node->children)
	{
		maxVal(child,alpha,beta,depth_left-1);
		beta =  min(beta,child->value);
		if (alpha>=beta) 
		{
			node->value = child->value;
		}
	}
	sort(node->children.begin(),node->children.end(), node_compare);
	node->value = node->children[0]->value;
}

void Bot::maxVal(Treenode* node, double alpha, double beta, int depth_left){
	if (depth_left==0||node->board->check_terminal())
		node->value = node->board->eval_func();
	for (Treenode* child: node->children)
	{
		minVal(child,alpha,beta, depth_left-1);
		alpha =  max(alpha,child->value);
		if (alpha>=beta) 
		{
			node->value = child->value;
		}
	}
	sort(node->children.begin(),node->children.end(), node_compare);
	node->value = node->children.back()->value;
}

void make_move(){
	
}