#include "bot.h"

int MAX_DEPTH;
int CHILDREN_THRESHOLD=35;
int DEEP_MAX_DEPTH=3;
int SHALLOW_MAX_DEPTH=2;
int SAVED_CHILDREN_CUTOFF = 3;
int BOARD_SIZE=5;

Bot::Bot(int player_id, double time_limit){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	check_board = new Board();
	this->player_id = player_id;
	this->time_left = time_limit;
	BOARD_SIZE = Board::board_size;
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
	if (depth_left==0||node->board->check_terminal()){
		node->value = eval_func(*node->board, player_id);
		return;
	}
	if (node->children.size()==0){
		node->generate_children();
	}
	double minValue = INT_MAX;
	for (int child=0; child < node->children.size(); child++){
		maxVal(node->children[child],alpha,beta,depth_left-1);
		beta =  min(beta,node->children[child]->value);

		if (alpha>=beta){
		 	node->value = node->children[child]->value;
		 	//cerr<<"PRUNED AT VALUE "<<child->value<<endl;
			return;
		}

		minValue = min(minValue,node->children[child]->value);
		
	}
	node->value=minValue;
	if (node->children.size()>0){
		if(depth_left < SAVED_CHILDREN_CUTOFF)
			node->delete_children();
	}

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
	
	if (depth_left==0||node->board->check_terminal()){
		node->value = eval_func(*node->board, player_id);
		return;
	}
	if (node->children.size()==0){
		node->generate_children();
	}

	double maxValue = INT_MIN;
	for (int child=0; child < node->children.size(); child++){

		minVal(node->children[child],alpha,beta, depth_left-1);
		alpha =  max(alpha,node->children[child]->value);
		if (alpha>=beta){
			node->value = node->children[child]->value;
		 	//cerr<<"PRUNED AT VALUE "<<child->value<<endl;
		 	return;
		}

		maxValue = max(maxValue,node->children[child]->value);
	}
	node->value = maxValue;

	if (node->children.size()>0){
		if(depth_left < SAVED_CHILDREN_CUTOFF)
			node->delete_children();
	}
	else{
		cerr<<"NO MOVES TO PLAY\n";
		while(1)
			cerr<<"NO MOVES TO PLAY\n";
	}
}

void Bot::read_move(){
	string move, spaced_move;
	getline(cin,move);

	double time_tick=clock();
	spaced_move = move + " ";	
	for (Treenode* child: root->children){
		if (child->move_description==move||child->move_description==spaced_move){
			root = child;
			return;
		}
	}
	
	// should generate the new config and remake tree
	root->board->execute_move(move);
	root->children.clear();
	root->generate_children();
	time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
	
}

location Bot::best_place_ring(){
	coordinates best_coord, c;
	location best_location, l;
	double max_block=INT_MIN, block;
	double randomising_seed;
	Board* newBoard = root->board->copy_board();

	for (int i=1;i<BOARD_SIZE;i++){
		l.hexagon=i;
		for (int j=0;j<6*i;j++){
			l.position=j;
			c = location_to_coordinates(l);
			if (root->board->board[c.x][c.y]!=0)
				continue;
			randomising_seed=double(rand())/RAND_MAX;
			newBoard->board[c.x][c.y]=2*player_id;
			block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
			if ((block>max_block && randomising_seed>0.5) || (block==max_block && randomising_seed>0.8)){
				max_block=block;
				best_coord.x=c.x;
				best_coord.y=c.y;
			}
			newBoard->board[c.x][c.y]=0;
		}
	}

	l.hexagon=BOARD_SIZE;
	for (int j=0;j<6*BOARD_SIZE;j++){
		l.position=j;
		c = location_to_coordinates(l);
		if (root->board->board[c.x][c.y]!=0)
			continue;
		randomising_seed=double(rand())/RAND_MAX;
		newBoard->board[c.x][c.y]=2*player_id;
		block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
		if (block>=max_block && randomising_seed>0.5){
			max_block=block;
			best_coord.x=c.x;
			best_coord.y=c.y;
		}
		newBoard->board[c.x][c.y]=0;
	}
	delete newBoard;
	best_location = coordinates_to_location(best_coord);
	return best_location;

}

void Bot::place_ring(){
	string move;
	
	if (player_id==1)
	{
		getline(cin,move);

		root->board->execute_move(move);
	}

	int rings_placed=0;
	location l;
	coordinates c;
	while(rings_placed<Board::given_rings){
		double time_tick = clock();
		rings_placed++;
		
		if (root->board->board[BOARD_SIZE][BOARD_SIZE]==0){
			move="P 0 0";
			root->board->execute_move(move);
			cout<<move<<endl;
		}
		else{
			l = best_place_ring();
			move="P "+to_string(l.hexagon)+" "+to_string(l.position);
			root->board->execute_move(move);
			cout<<move<<endl;
		}
		// root->board->printBoard();
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		getline(cin,move);
		
		root->board->execute_move(move);
		// root->board->printBoard();
	}
}

void Bot::play(){
	
	place_ring();

	int MAX_MOVES=100;
	while(MAX_MOVES>0){
		MAX_MOVES--;

		double time_tick=clock();
		
		if (player_id==1){
			Treenode* minNode;
			double minValue = INT_MAX;
			if(root->children.size() == 0)
				root->generate_children();
			if (root->children.size()>CHILDREN_THRESHOLD)
				MAX_DEPTH = SHALLOW_MAX_DEPTH;
			else
				MAX_DEPTH = DEEP_MAX_DEPTH;
			if (time_left<20)
				MAX_DEPTH=SHALLOW_MAX_DEPTH;
			for(int i = 0; i < root->children.size(); i++){
				maxVal(root->children[i], INT_MIN, INT_MAX, MAX_DEPTH);
				double value = root->children[i]->value;
				if (value < minValue){
					minValue = value;
					minNode = root->children[i];
				}
			}
			//minNode = minNode->copyNode();
			//delete root;
			root = minNode;
			cout << root->move_description<<endl;			
		}
		else{
			Treenode* maxNode;
			double maxValue = INT_MIN;
			if(root->children.size() == 0)
				root->generate_children();
			if (root->children.size()>CHILDREN_THRESHOLD)
				MAX_DEPTH = SHALLOW_MAX_DEPTH;
			else
				MAX_DEPTH = DEEP_MAX_DEPTH;
			if (time_left<20)
				MAX_DEPTH=SHALLOW_MAX_DEPTH;
			for(int i = 0; i < root->children.size(); i++){
				minVal(root->children[i], INT_MIN, INT_MAX, MAX_DEPTH);
				double value = root->children[i]->value;
				if (value > maxValue){
					maxValue = value;
					maxNode = root->children[i];
				}
			}
			//maxNode = maxNode->copyNode();
			//delete root;
			root = maxNode;
			cout << root->move_description<<endl;
			
		}
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;

		
		read_move();
	}
	
}