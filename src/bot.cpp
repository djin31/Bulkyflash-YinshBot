#include "bot.h"

int MAX_DEPTH=3;
int BOARD_SIZE=5;

Bot::Bot(int player_id, double time_limit){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	check_board = new Board();
	this->player_id = player_id;
	this->time_left = time_limit;

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
		node->value = node->board->eval_func(player_id);
		return;
	}
	if (node->children.size()==0)
	{
		node->generate_children();
		//cerr<<"MINVAL GENERATED CHILDREN "<<node->children.size()<<endl;
	}
	int children_seen=0;
	for (int child=0; child< node->children.size();child++)
	{
		children_seen=child;
		maxVal(node->children[child],alpha,beta,depth_left-1);
		beta =  min(beta,node->children[child]->value);

		// if (alpha>=beta) 
		// {
		// 	node->value = child->value;
		// 	// cerr<<"PRUNED AT VALUE "<<child->value<<endl;
		// 	// break;
		// }

	}
	sort(node->children.begin(),node->children.end(), node_compare);
	// cerr<<"CHILDREN VALUES\n";
	// for (Treenode* child:node->children)
	// 	cerr<<child->value<<" ";
	// cerr<<endl;
	if (node->children.size()>0)
		node->value = node->children.front()->value;
	else
	{
		cerr<<"NO MOVES TO PLAY\n";
		while(1)
		{
			cerr<<"NO MOVES TO PLAY\n";
		}
	}
}

void Bot::maxVal(Treenode* node, double alpha, double beta, int depth_left){
	if (depth_left==0||node->board->check_terminal())
	{
		node->value = node->board->eval_func(player_id);
		return;
	}
	if (node->children.size()==0)
	{
		node->generate_children();
	}
	int children_seen=0;
	for (int child=0; child< node->children.size();child++)
	{
		minVal(node->children[child],alpha,beta, depth_left-1);
		alpha =  max(alpha,node->children[child]->value);

		// if (alpha>=beta) 
		// {
		// 	node->value = child->value;
		// 	// cerr<<"PRUNED AT VALUE "<<child->value<<endl;
		// 	// break;
		// }

	}
	sort(node->children.begin(),node->children.end(), rev_node_compare);
	// cerr<<"CHILDREN VALUES\n";
	// for (Treenode* child:node->children)
	// 	cerr<<child->value<<" ";
	// cerr<<endl;
	if (node->children.size()>0)
		node->value = node->children.front()->value;
	else
	{
		cerr<<"NO MOVES TO PLAY\n";
		while(1)
		{
			cerr<<"NO MOVES TO PLAY\n";
		}
	}
}

void Bot::read_move(){
	string move, spaced_move;
	getline(cin,move);

	double time_tick=clock();
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
	time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
	
}

void Bot::place_ring(){
	string move;
	
	if (player_id==1)
	{
		getline(cin,move);

		root->board->execute_move(move);
		//cerr<<"Received first move\n";
	}
	int rings_placed=0;
	int rings_to_be_placed=5;
	location l;
	coordinates c;
	while(rings_placed<rings_to_be_placed){
		double time_tick = clock();
		rings_placed++;
		l.hexagon=rand()%(BOARD_SIZE+1);
		if (l.hexagon==0)
			l.position=0;
		else
			l.position=rand()%(6*l.hexagon);
		
		c=root->board->location_to_coordinates(l);
		while (!(root->board->checkValid(c) && root->board->board[c.x][c.y]==0))
		{
			l.hexagon=rand()%(BOARD_SIZE+1);
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
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		getline(cin,move);
		
		root->board->execute_move(move);
	}
}

void Bot::play(){
	
	place_ring();

	int MAX_MOVES=100;
	while(MAX_MOVES>0){
		MAX_MOVES--;

		double time_tick=clock();
		if (player_id==1)
		{
			minVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			if (root->children.size()>0){
				root = root->children.front();
				// for verification

				cout<<root->move_description<<endl;
			}
			
		}
		else{
			maxVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			if (root->children.size()>0){
				root = root->children.front();

				cout<<root->move_description<<endl;
			}
			
		}
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		
		read_move();

		if (time_left<10)
			MAX_DEPTH=2;

	}
	
}