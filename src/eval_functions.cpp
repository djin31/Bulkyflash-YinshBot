#include "eval_functions.h"

void set_eval_params(vector<double> params){
	RING_WEIGHTS = params[0];
	MARKER_WEIGHTS = params[1];
	BLOCKING_WEIGHT = params[2];
	WEIGHT_MARKERS_IN_LINE = params[3];
	NORMALIZE_WEIGHT = params[4];
	WEIGHT_TO_RING_IN_LINE = params[5];
}

double eval_func(const Board &board, int player_id){
	double hugeNumber = 1000000;

	if (board.white_rings_out>=board.rings_to_remove)
		return hugeNumber;
	else if (board.black_rings_out>=board.rings_to_remove)
		return -hugeNumber;

	double ring_weights = RING_WEIGHTS, marker_weights = MARKER_WEIGHTS, blocking_weight=BLOCKING_WEIGHT;
	double score;

	int rings_blocked_by_white = 0, rings_blocked_by_black=0;
	for (coordinates c: board.white_rings)
		rings_blocked_by_white+=blocked_rings(board.board, c);
	
	for (coordinates c: board.black_rings)
		rings_blocked_by_black+=blocked_rings(board.board, c);

	
	score = ring_weights*(board.white_rings_out-board.black_rings_out) + marker_weights*(board.white_markers-board.black_markers) + blocking_weight*(rings_blocked_by_white-rings_blocked_by_black) + eval_markers_in_row(board.board, board.board_size);
	//score+= controlled_markers_in_row();
	if (player_id==-1)
		score+=ring_weights*board.white_rings_out;
	else
		score-=ring_weights*board.black_rings_out;
	return score;
}

int blocked_rings(const vector<vector<int>> &board, coordinates c){
	int ring_id = board[c.x][c.y];

	coordinates it;
	int retVal=0;

	//moving up
	it.x=c.x;
	it.y=c.y-1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.y--;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;
	
	//moving down
	it.x=c.x;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.y++;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;

	//moving right
	it.x=c.x+1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.x++;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;

	//moving left
	it.x=c.x-1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.x--;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;

	// moving right down
	it.x=c.x+1;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
	{	
		it.x++;it.y++; 
	}
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;

	// moving left up
	it.x=c.x-1;
	it.y=c.y-1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
	{	
		it.x--;it.y--; 
	}
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	if (!checkValid(it))
		retVal-=1;
	return retVal;
}

int self_blocked_rings(const vector<vector<int>> &board, coordinates c){
	int ring_id = board[c.x][c.y];

	coordinates it;
	int retVal=0;

	//moving up
	it.x=c.x;
	it.y=c.y-1;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
		it.y--;
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];
	
	//moving down
	it.x=c.x;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
		it.y++;
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];

	//moving right
	it.x=c.x+1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
		it.x++;
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];

	//moving left
	it.x=c.x-1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
		it.x--;
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];

	// moving right down
	it.x=c.x+1;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
	{	
		it.x++;it.y++; 
	}
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];

	// moving left up
	it.x=c.x-1;
	it.y=c.y-1;
	while (checkValid(it) && abs(board[it.x][it.y])<=1)
	{	
		it.x--;it.y--; 
	}
	if (checkValid(it) && board[it.x][it.y]==ring_id)
		retVal-=ring_id/board[it.x][it.y];
	return retVal;
}

double eval_markers_in_row(const vector<vector<int>> &board, int board_size){
	double weight_to_ring=WEIGHT_TO_RING_IN_LINE;
	double retVal=0;
	double counter_value[] = {1, 3, 9, 27, 81, 100, 150, 220, 310, 410, 550};
	coordinates start_coord,end_coord;
	
	for (int i=0;i<2*board_size+1;i++){
		for (int j=0;j<2*board_size+1;j++){
			
			start_coord.x=i;
			start_coord.y=j;
			if (!(checkValid(start_coord) && abs(board[i][j])==1))
				continue;
			
			//moving down
			// cerr<<"check down"<<endl;
			int counter=1,k=j+1,l;
			end_coord.x=i;
			end_coord.y=k;

			while(checkValid(end_coord))
			{
				if (board[i][k]!=board[i][j])
					break;
				counter++;
				end_coord.y=++k;
				
			}
			// retVal - since white has negative turn id
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;

			// moving up
			// cerr<<"check up"<<endl;
			end_coord.x=i;
			end_coord.y=k;

			counter=1;
			k=j-1;
			while(checkValid(end_coord))
			{
				if (board[i][k]!=board[i][j])
					break;
				counter++;
				end_coord.y=--k;
				
			}
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;

			// moving right sideways
			// cerr<<"check right"<<endl;
			counter=1;
			k=i+1;
			end_coord.x=k;
			end_coord.y=j;
			while(checkValid(end_coord))
			{
				if (board[k][j]!=board[i][j])
					break;
				counter++;
				end_coord.x=++k;
				
			}
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[k][i]*counter_value[counter]*weight_to_ring;

			// moving left sideways
			// cerr<<"check left"<<endl;
			counter=1;
			k=i-1;
			end_coord.x=k;
			end_coord.y=j;
			while(checkValid(end_coord))
			{
				if (board[k][j]!=board[i][j])
					break;
				counter++;
				end_coord.x=--k;
				
			}
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[k][i]*counter_value[counter]*weight_to_ring;
			
			// moving diagonal right down
			// cerr<<"check diagonal right"<<endl;
			counter=1;
			k=i+1;
			l=j+1;
			end_coord.x=k;
			end_coord.y=l;
			while(checkValid(end_coord))
			{
				if (board[k][l]!=board[i][j])
					break;
				counter++;
				end_coord.x=++k;
				end_coord.y=++l;
				
			}
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[k][l]*counter_value[counter]*weight_to_ring;

			// moving diagonal right down
			// cerr<<"check diagonal leftt"<<endl;
			
			counter=1;
			k=i-1;
			l=j-1;
			end_coord.x=k;
			end_coord.y=l;
			while(checkValid(end_coord))
			{
				if (board[k][l]!=board[i][j])
					break;
				counter++;
				end_coord.x=--k;
				end_coord.y=--l;
				
			}
			retVal-=board[i][j]*counter_value[counter]*WEIGHT_MARKERS_IN_LINE;
			if (checkValid(end_coord))
				retVal-=board[k][l]*counter_value[counter]*weight_to_ring;

		}
	}
	return retVal;
}