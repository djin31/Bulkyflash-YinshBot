#include "bot.h"

int MAX_DEPTH;
#define CHILDREN_THRESHOLD 35;
#define DEEP_MAX_DEPTH 1;
#define SHALLOW_MAX_DEPTH 1;
#define SAVED_CHILDREN_CUTOFF 3;

Bot::Bot(int player_id, double time_limit){  
	this->board = new Board();
	this->player_id = player_id;
	this->time_left = time_limit;
    srand(time(NULL));
}

double Bot::minVal(Board *board, double alpha, double beta, int depth_left){
	if (depth_left==0||board->check_terminal()){
		return eval_func(*board, player_id);
	}
	double minValue = INT_MAX;
	
	vector<string> children = board->get_valid_actions();
	
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
		return eval_func(*board, player_id);
	}
	double maxValue = INT_MIN;
	
	vector<string> children = board->get_valid_actions();

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
	double max_block=INT_MIN, block;
	double randomising_seed;
	Board* newBoard = board->copy_board();

	for (int i=1;i<board_size;i++){
		l.hexagon=i;
		for (int j=0;j<6*i;j++){
			l.position=j;
			c = location_to_coordinates(l);
			if (board->board[c.x][c.y]!=0)
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

	l.hexagon=board_size;
	for (int j=0;j<6*board_size;j++){
		l.position=j;
		c = location_to_coordinates(l);
		if (board->board[c.x][c.y]!=0)
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

		board->execute_move(move);
	}

	int rings_placed=0;
	location l;
	coordinates c;
	while(rings_placed<Board::given_rings){
		double time_tick = clock();
		rings_placed++;
		
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
		// root->board->printBoard();
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		getline(cin,move);
		
		board->execute_move(move);
		// root->board->printBoard();
	}
}

void Bot::minimax_decision(){
	MAX_DEPTH = DEEP_MAX_DEPTH;
	if (time_left<20)
		MAX_DEPTH=SHALLOW_MAX_DEPTH;

	if (player_id==1){
		double minValue = INT_MAX;
		string minAction = "";
		vector<string> children = this->board->get_valid_actions();
		for( auto s : children ){
			Board tempBoard = *board;
			tempBoard.execute_move(s);
			double val = maxVal(&tempBoard, INT_MIN, INT_MAX, MAX_DEPTH);
			if(minValue > val){
				minValue = val;
				minAction = s;
			}
		}
		this->board->execute_move(minAction);
		cout << minAction << "\n";
	}
	else{
		double maxValue = INT_MIN;
		string maxAction = "";
		vector<string> children = this->board->get_valid_actions();
		for( auto s : children ){
			Board tempBoard = *board;
			tempBoard.execute_move(s);
			double val = minVal(&tempBoard, INT_MIN, INT_MAX, MAX_DEPTH);
			if(maxValue < val){
				maxValue = val;
				maxAction = s;
			}
		}
		this->board->execute_move(maxAction);
		cout << maxAction << "\n";
	}
}

void Bot::play(){
	place_ring();
	int MAX_MOVES=100;

	cerr << "STRESS TEST\n";
	cerr << "TIME FOR 1000 execute move\n";
	double time_start=clock();
	string move = "P 0 0";
	this->board->execute_move(move);
	for(int i = 0; i < 500; i++){
		move = "S 0 0 M 4 2";
		this->board->execute_move(move);
		move = "S 4 2 M 0 0";
		this->board->execute_move(move);
	}
	double time_end = clock();
	cerr << "time taken : " << (time_end - time_start)/CLOCKS_PER_SEC << "\n\n";

	cerr << "TIME FOR 100 get_valid_actions\n";
	time_start=clock();
	for(int i = 0; i < 100; i++){
		vector<string> children = this->board->get_valid_actions();
	}
	time_end = clock();
	cerr << "time taken : " << (time_end - time_start)/CLOCKS_PER_SEC << "\n\n";

	cerr << "TIME FOR 100 eval_func\n";
	time_start=clock();
	for(int i = 0; i < 100; i++){
		eval_func(*board, player_id);
	}
	time_end = clock();
	cerr << "time taken : " << (time_end - time_start)/CLOCKS_PER_SEC << "\n";

	while(MAX_MOVES>0){
		MAX_MOVES--;
		double time_tick=clock();
		minimax_decision();
		time_left -= ((double)(clock()-time_tick))/CLOCKS_PER_SEC;
		read_move();
	}
}