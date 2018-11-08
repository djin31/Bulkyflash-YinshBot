#include "eval_functions.h"

void set_eval_params(vector<double> params){
	if (params.size()==5)
	{	
		MARKER_WEIGHTS = params[0];
		BLOCKING_WEIGHT = params[1];
		WEIGHT_MARKERS_IN_LINE = params[2];
		WEIGHT_TO_RING_IN_LINE = params[3];
		CONTROL_MARKERS = params[4];
	}
	else
	{
		cerr<<"Incorrect parameters for eval\n";
	}
}

double eval_func(const Board &board, int player_id){

	double score = 0.0;

	if (board.white_rings_out>=board.rings_to_remove)
		score+= hugeNumber;
	else if (board.black_rings_out>=board.rings_to_remove)
		score-=hugeNumber;



	int rings_blocked_by_white = 0, rings_blocked_by_black=0;
	for (coordinates c: board.white_rings)
		rings_blocked_by_white+=blocked_rings(board.board, c);
	
	for (coordinates c: board.black_rings)
		rings_blocked_by_black+=blocked_rings(board.board, c);

	
	score += RING_WEIGHTS*(board.white_rings_out-board.black_rings_out) ;
	score += MARKER_WEIGHTS*(board.white_markers-board.black_markers);
	//score += BLOCKING_WEIGHT*(rings_blocked_by_white-rings_blocked_by_black);
	score += eval_markers_in_row(board.board, board.board_size);
	//score += CONTROL_MARKERS*control_markers(board.board, board.board_size);
	//cerr << "*****\n";
	//cerr << RING_WEIGHTS*(board.white_rings_out-board.black_rings_out) << "\n";
	//cerr << MARKER_WEIGHTS*(board.white_markers-board.black_markers) << "\n";

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
	double counter_value[] = {1, 2, 4, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 256};
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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

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
			if (checkValid(end_coord)){
				retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
				if (board[i][k]==2*board[i][j])
					retVal-=board[i][k]*counter_value[counter]*weight_to_ring;
			}

		}
	}
	return retVal;
}

double control_markers(const vector<vector<int>> &board, int board_size){
	double retVal=0;
	double counter_value[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	coordinates start_coord,end_coord;
	bool vertical=false,horizontal=false,diagonal=false;
	for (int i=0;i<2*board_size+1;i++){
		for (int j=0;j<2*board_size+1;j++){
			
			start_coord.x=i;
			start_coord.y=j;
			if (!(checkValid(start_coord) && abs(board[i][j])==1))
				continue;

			vertical=false; horizontal=false; diagonal=false;

			//moving down
			// cerr<<"check down"<<endl;
			int k=j+1,l;
			end_coord.x=i;
			end_coord.y=k;

			while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
			{
				end_coord.y=++k;				
			}
			if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
				continue;
			else
				vertical = true;
			
			//moving up
			// cerr<<"check up"<<endl;
			if (!vertical){
				int k=j-1,l;
				end_coord.x=i;
				end_coord.y=k;

				while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
				{
					end_coord.y=--k;				
				}
				if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
					continue;
				else
					vertical = true;
			}

			// moving right sideways
			// cerr<<"check right"<<endl;
			k=i+1;
			end_coord.x=k;
			end_coord.y=j;

			while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
			{
				end_coord.x=++k;				
			}
			if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
				continue;
			else
				horizontal=true;

			
			// moving left sideways
			// cerr<<"check left"<<endl;
			if (!horizontal){	
				k=i-1;
				end_coord.x=k;
				end_coord.y=j;

				while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
				{
					end_coord.x=--k;				
				}
				if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
					continue;
				else
					horizontal=true;
			}

			// moving diagonal right down
			// cerr<<"check diagonal right"<<endl;
			k=i+1;
			l=j+1;
			end_coord.x=k;
			end_coord.y=l;
			while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
			{
				end_coord.x=++k;
				end_coord.y=++l;
				
			}
			if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
				continue;
			else
				diagonal=true;

			// moving diagonal right down
			// cerr<<"check diagonal leftt"<<endl;
			if (!diagonal){	
				k=i-1;
				l=j-1;
				end_coord.x=k;
				end_coord.y=l;
				while(checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==1)
				{
					end_coord.x=--k;
					end_coord.y=--l;
					
				}
				if (checkValid(end_coord) && abs(board[end_coord.x][end_coord.y])==0)
					continue;
				else
					diagonal=true;
			}
			if (vertical && diagonal && horizontal)
				retVal-=board[i][j];
		}
	}
	return retVal;
}