#include "bot.h"

int MAX_DEPTH=3;

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
		node->value = node->board->eval_func();
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
		if (alpha>=beta) 
		{
			node->value = node->children[child]->value;
			if (children_seen<node->children.size()-1)
				//cerr<<"PRUNED AT DEPTH "<<depth_left<<endl;
			break;
		}
	}
	sort(node->children.begin(),node->children.begin()+children_seen, node_compare);
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
	int children_seen=0;
	for (int child=0; child< node->children.size();child++)
	{
		minVal(node->children[child],alpha,beta, depth_left-1);
		alpha =  max(alpha,node->children[child]->value);
		if (alpha>=beta) 
		{
			node->value = node->children[child]->value;
			if (children_seen<node->children.size()-1)
				//cerr<<"PRUNED AT DEPTH "<<depth_left<<endl;
			break;
		}
	}
	sort(node->children.begin(),node->children.begin()+children_seen, rev_node_compare);
	node->value = node->children.front()->value;
}

void Bot::read_move(){
	string move, spaced_move;
	getline(cin,move);

	// for verification
		//cerr<<"CHECKED MOVE\n";

	//check_board->execute_move(move);
		//cerr<<"CHECKED MOVE2\n";

	double time_tick=clock();
	spaced_move = move + " ";	
	for (Treenode* child: root->children){
		if (child->move_description==move||child->move_description==spaced_move){
			root = child;return;
		}
	}
	////cerr<<"OPPONENT'S MOVE NOT FOUND AMONGST CHILDREN\n";
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
		// for verification
		//cerr<<"CHECKED MOVE\n";

		//check_board->execute_move(move);
		//cerr<<"CHECKED MOVE2\n";

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
		l.hexagon=rand()%6;
		if (l.hexagon==0)
			l.position=0;
		else
			l.position=rand()%(6*l.hexagon);
		
		c=root->board->location_to_coordinates(l);
		while (!(root->board->checkValid(c) && root->board->board[c.x][c.y]==0))
		{
			l.hexagon=rand()%6;
			if (l.hexagon==0)
				l.position=0;
			else
				l.position=rand()%(6*l.hexagon);
			c=root->board->location_to_coordinates(l);
		}
		move = "P " + to_string(l.hexagon) + " " + to_string(l.position);
		// for verification
		//cerr<<"CHECKED MOVE\n";
		//check_board->execute_move(move);
		//cerr<<"CHECKED MOVE2\n";

		root->board->execute_move(move);
		cout<<move<<endl;
		// root->board->printBoard();
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		getline(cin,move);
		// for verification
		//cerr<<"CHECKED MOVE\n";
		//check_board->execute_move(move);
		//cerr<<"CHECKED MOVE2\n";
		
		root->board->execute_move(move);
		root->board->printBoard();
	}
}

void Bot::play(){
	
	place_ring();

	while(true){
		double time_tick=clock();
		if (player_id==1)
		{
			minVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			if (root->children.size()>0){
				root = root->children.front();
				// for verification
		//cerr<<"CHECKED MOVE\n";

				//check_board->execute_move(root->move_description);
		//cerr<<"CHECKED MOVE2\n";
				cerr<<root->move_description<<endl;
				root->board->printBoard();
				cout<<root->move_description<<endl;
			}
			//else
				//cerr<<"NO CHILDREN FOUND\n";
		}
		else{
			maxVal(root,INT_MIN, INT_MAX, MAX_DEPTH);
			if (root->children.size()>0){
				root = root->children.front();
				// for verification
		//cerr<<"CHECKED MOVE\n";

				//check_board->execute_move(root->move_description);
		//cerr<<"CHECKED MOVE2\n";
				cerr<<root->move_description<<endl;
				root->board->printBoard();
				cout<<root->move_description<<endl;
			}
			//else
				//cerr<<"NO CHILDREN FOUND\n";
		}
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		
		read_move();

		if (time_left<10)
			MAX_DEPTH=2;

	}
	
}