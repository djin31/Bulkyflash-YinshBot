#include "bot.h"

int MAX_DEPTH=4;

Bot::Bot(int player_id, double time_limit){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	this->player_id = player_id;
	this->time_limit = time_limit;
}

bool node_compare(Treenode* a, Treenode* b)
{
	return (a->value < b->value);
}

void Bot::minVal(Treenode* node, double alpha, double beta, int depth_left){
	if (depth_left==0||node->board->check_terminal())
		node->value = node->board->eval_func();
	if (node->children.size()==0)
	{
		node->generate_children(1);
	}
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
	if (node->children.size()==0)
	{
		node->generate_children(0);
	}
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

void Bot::read_move(){
	string move;
	getline(cin,move);
		
	for (Treenode* child: root->children){
		if (child->move_description==move){
			root = child;return;
		}
	}
	cout<<"OPPONENT'S MOVE NOT FOUND AMONGST CHILDREN\n";
	// should generate the new config and remake tree

	Treenode* new_root = new Treenode();
	new_root->board = root
}

void Bot::play(){
	string move;
	if (player_id==1)
	{
		read_move();
	}

	while(true){
		if (player_id==1)
		{
			minVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			root = root->children.front();
			cout<<root->move_description<<endl;
		}
		else{
			maxVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			root = root->children.back();
			cout<<root->move_description<<endl;
		}
		read_move();
	}
}