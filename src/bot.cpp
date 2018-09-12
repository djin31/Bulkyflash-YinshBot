#include "bot.h"

int MAX_DEPTH=2;

Bot::Bot(int player_id, double time_limit){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	this->player_id = player_id;
	this->time_limit = time_limit;

    srand(time(NULL));

}

bool node_compare(Treenode* a, Treenode* b)
{
	return (a->value < b->value);
}
bool rev_node_compare(Treenode* a, Treenode* b)
{
	return (a->value > b->value);
}

void Bot::minVal(Treenode* node, double alpha, double beta, int depth_left){
	if (depth_left==0||node->board->check_terminal())
	{
		node->value = node->board->eval_func();
		return;
	}
	if (node->children.size()==0)
	{
		node->generate_children();
		cerr<<"MINVAL GENERATED CHILDREN "<<node->children.size()<<endl;
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
	{
		node->value = node->board->eval_func();
		return;
	}
	if (node->children.size()==0)
	{
		node->generate_children();
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
	sort(node->children.begin(),node->children.end(), rev_node_compare);
	node->value = node->children.front()->value;
}

void Bot::read_move(){
	string move, spaced_move;
	getline(cin,move);
	spaced_move = move + " ";	
	for (Treenode* child: root->children){
		if (child->move_description==move||child->move_description==spaced_move){
			root = child;return;
		}
	}
	cerr<<"OPPONENT'S MOVE NOT FOUND AMONGST CHILDREN\n";
	// should generate the new config and remake tree
	root->board->execute_move(move);
	root->children.clear();
	root->generate_children();
	
}

void Bot::place_ring(){
	string move;
	
	if (player_id==1)
	{
		getline(cin,move);
		root->board->execute_move(move);
		cerr<<"Received first move\n";
	}
	int rings_placed=0;
	int rings_to_be_placed=5;
	location l;
	coordinates c;
	while(rings_placed<rings_to_be_placed){
		rings_placed++;
		l.hexagon=rand()%6;
		if (l.hexagon==0)
			l.position=0;
		else
			l.position=rand()%(6*l.hexagon);
		c=root->board->location_to_coordinates(l);
		while (root->board->board[c.x][c.y]!=0)
		{
			l.hexagon=rand()%6;
			if (l.hexagon==0)
				l.position=0;
			else
				l.position=rand()%(6*l.hexagon);
			c=root->board->location_to_coordinates(l);
		}
		move = "P " + to_string(l.hexagon) + " " + to_string(l.position);
		root->board->execute_move(move);
		cout<<move<<endl;
		// root->board->printBoard();

		getline(cin,move);
		root->board->execute_move(move);
	}
}

void Bot::play(){
	
	place_ring();

	while(true){
		if (player_id==1)
		{
			minVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			root = root->children.front();
			for (int i=0;i<5;i++)
				cerr<<i<<" ring "<<root->board->black_rings[i].x<<" "<<root->board->black_rings[i].y<<endl;
			cout<<root->move_description<<endl;
		}
		else{
			maxVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			root = root->children.front();
			cout<<root->move_description<<endl;
		}
		read_move();
	}
	
}