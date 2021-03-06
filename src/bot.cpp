#include "bot.h"

int MAX_DEPTH;
static int CHILDREN_THRESHOLD = 50;
static int DEEP_MAX_DEPTH = 3;
static int SHALLOW_MAX_DEPTH = 2;
static int EMERGENCY_DEPTH = 1;
#define SAVED_CHILDREN_CUTOFF 3;
#define MIN_VAL -10000000000.0
#define MAX_VAL 10000000000.0

long long nodes_seen = 0;

Bot::Bot(int player_id, double time_limit){  
	this->board = new Board();
	this->player_id = player_id;
	this->time_left = time_limit;
    srand(time(NULL));
}

double Bot::minVal(Board *board, double alpha, double beta, int depth_left){
	if (depth_left==0||board->check_terminal()){
		return eval_func(*board, player_id)-player_id*depth_left*100;
	}
	double minValue = MAX_VAL;
	
	vector<string> children = board->get_valid_actions();
	nodes_seen += children.size();
	for( auto s : children ){
		Board tempBoard = *board;
		tempBoard.execute_move(s);
		double val = maxVal(&tempBoard, alpha, beta, depth_left-1);
		beta =  min(beta, val);
		if (alpha>=beta){
			return val;
		}
		minValue = min(minValue, val);
	}

	return minValue;
}

double Bot::maxVal(Board *board, double alpha, double beta, int depth_left){
	if (depth_left==0||board->check_terminal()){
		return eval_func(*board, player_id)-player_id*depth_left*100;
	}
	double maxValue = MIN_VAL;
	
	vector<string> children = board->get_valid_actions();
	nodes_seen += children.size();
	for (auto s : children){
		Board tempBoard = *board;
		tempBoard.execute_move(s);
		double val = minVal(&tempBoard, alpha, beta, depth_left-1);
		alpha =  max(alpha, val);
		if (alpha>=beta){
		 	return val;
		}
		maxValue = max(maxValue, val);
	}

	return maxValue;
}

void Bot::read_move(){
	string move, spaced_move;
	getline(cin,move);

	double time_tick=clock();
	spaced_move = move + " ";	
	
	board->execute_move(move);
	time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
}

location Bot::best_place_ring(){
	coordinates best_coord, c;
	location best_location, l;
	double max_block=MIN_VAL, block;
	double randomising_seed;
	Board* newBoard = board->copy_board();

	// randomised policy
	// for (int i=1;i<board_size;i++){
	// 	l.hexagon=i;
	// 	for (int j=0;j<6*i;j++){
	// 		l.position=j;
	// 		c = location_to_coordinates(l);
	// 		if (board->board[c.x][c.y]!=0)
	// 			continue;
	// 		randomising_seed=double(rand())/RAND_MAX;
	// 		newBoard->board[c.x][c.y]=2*player_id;
	// 		block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
	// 		if ((block>max_block && randomising_seed>0.5) || (block==max_block && randomising_seed>(0.8 + 0.02*i))){
	// 			max_block=block;
	// 			best_coord.x=c.x;
	// 			best_coord.y=c.y;
	// 		}
	// 		newBoard->board[c.x][c.y]=0;
	// 	}
	// }

	// l.hexagon=board_size;
	// for (int j=0;j<6*board_size;j++){
	// 	l.position=j;
	// 	c = location_to_coordinates(l);
	// 	if (board->board[c.x][c.y]!=0)
	// 		continue;
	// 	randomising_seed=double(rand())/RAND_MAX;
	// 	newBoard->board[c.x][c.y]=2*player_id;
	// 	block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
	// 	if (block>=max_block && randomising_seed>0.5){
	// 		max_block=block;
	// 		best_coord.x=c.x;
	// 		best_coord.y=c.y;
	// 	}
	// 	newBoard->board[c.x][c.y]=0;
	// }

	// deterministic for deterministic bots
	l.hexagon=board_size;
	for (int j=0;j<6*board_size;j++){
		l.position=j;
		c = location_to_coordinates(l);
		if (board->board[c.x][c.y]!=0)
			continue;
		newBoard->board[c.x][c.y]=2*player_id;
		block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
		if (block>=max_block){
			max_block=block;
			best_coord.x=c.x;
			best_coord.y=c.y;
		}
		newBoard->board[c.x][c.y]=0;
	}

	for (int i=1;i<board_size;i++){
		l.hexagon=i;
		for (int j=0;j<6*i;j++){
			l.position=j;
			c = location_to_coordinates(l);
			if (board->board[c.x][c.y]!=0)
				continue;
			newBoard->board[c.x][c.y]=2*player_id;
			block = blocked_rings(newBoard->board, c) + 2*self_blocked_rings(newBoard->board, c);
			if (block>=max_block){
				max_block=block;
				best_coord.x=c.x;
				best_coord.y=c.y;
			}
			newBoard->board[c.x][c.y]=0;
		}
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

		board->execute_move(move);
	}

	int rings_placed=0;
	location l;
	coordinates c;
	int pot_positions[7] = {0,7,8,9,11,10,12};
	bool use_pot_positions=false;
	if (board_size==6 && Board::markers_in_line==5){
		use_pot_positions=true;		
	}

	while(rings_placed<Board::given_rings){
		double time_tick = clock();
		rings_placed++;
		
		if (use_pot_positions){			
			for (int i=7;i<12;i++){
				l.hexagon = 6;
				l.position = i;
				c = location_to_coordinates(l);
				if (board->board[c.x][c.y]==(-2*player_id) || board->board[c.x][c.y]==(-1*player_id)){
					use_pot_positions=false;
				}
				
			}
			
			l.hexagon = 6;
			l.position = pot_positions[rings_placed];
			c = location_to_coordinates(l);
			
			if (checkValid(c) && board->board[c.x][c.y]==0 && use_pot_positions){
				
				move = "P " + to_string(6) + " " + to_string(pot_positions[rings_placed]);
				board->execute_move(move);
				cout<<move<<endl;
			}
			else{
				use_pot_positions=false;
			}
		}

		if (!use_pot_positions){
			if (board->board[board_size][board_size]==0){
				move="P 0 0";
				board->execute_move(move);
				cout<<move<<endl;
			}
			else{
				l = best_place_ring();
				move="P "+to_string(l.hexagon)+" "+to_string(l.position);
				board->execute_move(move);
				cout<<move<<endl;
			}
		}
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		getline(cin,move);
		
		board->execute_move(move);
		
	}
}

void Bot::minimax_decision(int MOVE_NUMBER){
	nodes_seen = 0;
	MAX_DEPTH = DEEP_MAX_DEPTH;
	if (time_left<40|| MOVE_NUMBER<15)
		MAX_DEPTH=SHALLOW_MAX_DEPTH;

	if (player_id==1){
		double minValue = MAX_VAL;
		string minAction = "";
		vector<string> children = this->board->get_valid_actions();
		nodes_seen += children.size();
		if (children.size()> CHILDREN_THRESHOLD){
			MAX_DEPTH=SHALLOW_MAX_DEPTH;
		}
		if (time_left<10){
			MAX_DEPTH = EMERGENCY_DEPTH;
		}
		for( auto s : children ){
			// cerr << s << "\t: ";
			Board tempBoard = *board;
			tempBoard.execute_move(s);
			double val = maxVal(&tempBoard, MIN_VAL, MAX_VAL, MAX_DEPTH);
			// cerr << val << "\n";
			if(minValue > val){
				minValue = val;
				minAction = s;
			}
		}
		this->board->execute_move(minAction);
		cout << minAction << "\n";
		cerr << "### " << minAction << " "<<minValue<<" at depth "<<MAX_DEPTH<<" with time left "<<time_left<<"\n";
	}
	else{
		double maxValue = MIN_VAL;
		string maxAction = "";
		vector<string> children = this->board->get_valid_actions();
		nodes_seen += children.size();
		if (children.size()> CHILDREN_THRESHOLD){
			MAX_DEPTH=SHALLOW_MAX_DEPTH;
		}
		if (time_left<10){
			MAX_DEPTH = EMERGENCY_DEPTH;
		}
		for( auto s : children ){
			// cerr << s << "\t: ";
			Board tempBoard = *board;
			tempBoard.execute_move(s);
			double val = minVal(&tempBoard, MIN_VAL, MAX_VAL, MAX_DEPTH);
			// cerr << val << "\n";
			if(maxValue < val){
				maxValue = val;
				maxAction = s;
			}
		}
		this->board->execute_move(maxAction);
		cout << maxAction << "\n";
		cerr << "### " << maxAction << " "<<maxValue<<" at depth "<<MAX_DEPTH<<" with time left "<<time_left<<"\n";
	}
	//cerr << "nodes seen : " << nodes_seen << "\n";
}

void Bot::play(){
	place_ring();
	int MAX_MOVES=100;
	if (board_size>=5)
		DEEP_MAX_DEPTH = 2;
	while(MAX_MOVES>0){
		MAX_MOVES--;
		double time_tick=clock();
		// board->printBoard();
		minimax_decision(100-MAX_MOVES);
		// board->printBoard();
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		read_move();
	}
}