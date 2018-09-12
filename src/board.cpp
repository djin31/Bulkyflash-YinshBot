#include "board.h"

Board::Board(){

	//vector<vector<int>> board;		// 0 -> empty, 1 -> black marker, 2 -> black ring, -1 -> white marker, -2 -> white ring
	// this->board_size = board_size;			// N
	// this->given_rings = given_rings;		// M
	// this->rings_to_remove = rings_to_remove;	// L
	// this->markers_in_line = markers_in_line;	// K
	
	black_markers = 0;
	white_markers = 0;
	black_rings_in = 0; 
	white_rings_in = 0;
	black_rings_out = 0;
	white_rings_out = 0;
	
	turn_id = -1;	// -1 << white, 1 << black
	current_score = 0.0;

	// initialise board to all zeroes
	vector<int> zeroes(2*board_size+1,0);
	
	for (int i=0;i<2*board_size + 1;i++)
		board.push_back(zeroes);
}

bool Board::placeRing(int ring_no, coordinates c){
	if(c.x > 2*board_size || c.y >= 2*board_size)
		return false;
	
	board[c.x][c.y] = ring_no;
	if (ring_no == 2){
		black_rings_in++; black_rings.push_back(c);
	}
	else{
		white_rings_in++; white_rings.push_back(c);
	}
	
	return true;
}

bool Board::moveRing(coordinates start, coordinates end){
	int ring_no = board[start.x][start.y];
	board[start.x][start.y] = ring_no/2;
	board[end.x][end.y] = ring_no;
	
	//changing ring vectors
	if(ring_no == 2){
		for(int i = 0; i < black_rings.size(); i++){
			if(black_rings[i].x == start.x && black_rings[i].y == start.y){
				black_rings[i].x = end.x;
				black_rings[i].y = end.y;
				break;
			}
		}
	}
	else{
		for(int i = 0; i < white_rings.size(); i++){
			if(white_rings[i].x == start.x && white_rings[i].y == start.y){
				white_rings[i].x = end.x;
				white_rings[i].y = end.y;
				break;
			}
		}
	}


	if(ring_no == 2)
		black_markers++;
	else
		white_markers++;
	if(start.x == end.x){
		if(start.y <= end.y)
			iterate_over_line(-1, 1, start.x, start.y+1, end.y-1);
		else
			iterate_over_line(-1, 1, start.x, end.y+1, start.y-1);
		return true;
	}	
	if(start.y == end.y){
		if(start.x <= end.x)
			iterate_over_line(-1, 0, start.y, start.x+1, end.x-1);
		else
			iterate_over_line(-1, 0, start.y, end.x+1, start.x-1);
		return true;
	}
	if(start.x - start.y == end.x - end.y){
		if(start.x <= end.x){
			for(int i=  1; i < end.x - start.x; i++){
				if(board[start.x + i][start.y + i] == 1)
					black_markers--;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers--;
				board[start.x + i][start.y + i] *= -1;
				if(board[start.x + i][start.y + i] == 1)
					black_markers++;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers++;
			}
		}
		else{
			for(int i=  1; i < start.x - end.x; i++){
				if(board[end.x + i][end.y + i] == 1)
					black_markers--;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers--;
				board[end.x + i][end.y + i] *= -1;	
				if(board[end.x + i][end.y + i] == 1)
					black_markers++;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers++;
			}
		}
		return true;	
	}
	return false;
}

bool Board::removeMarkerSeq(coordinates start, coordinates end){
	if(start.x == end.x){
		if(start.y <= end.y)
			iterate_over_line(0, 1, start.x, start.y, end.y);
		else
			iterate_over_line(0, 1, start.x, end.y, start.y);
		return true;
	}	
	if(start.y == end.y){
		if(start.x <= end.x)
			iterate_over_line(0, 0, start.y, start.x, end.x);
		else
			iterate_over_line(0, 0, start.y, end.x, start.x);
		return true;
	}
	if(start.x - start.y == end.x - end.y){
		if(start.x <= end.x){
			for(int i=  0; i <= end.x - start.x; i++){
				if(board[start.x + i][start.y + i] == 1)
					black_markers--;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers--;
				board[start.x + i][start.y + i] = 0;
			}
		}
		else{
			for(int i=  0; i <= start.x - end.x; i++){
				if(board[end.x + i][end.y + i] == 1)
					black_markers--;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers--;
				board[end.x + i][end.y + i] = 0;	
			}
		}
		return true;	
	}
	return false;
}

bool Board::removeRing(coordinates c){
	board[c.x][c.y] = 0;
	if (turn_id == -1){
		white_rings_in--; white_rings_out++;
		for(int i = 0; i < white_rings.size(); i++){
			if(c.x == white_rings[i].x && c.y == white_rings[i].y)
				white_rings.erase(white_rings.begin() + i);
		}
	}
	else{
		black_rings_in--; black_rings_out++;
		for(int i = 0; i < black_rings.size(); i++){
			if(c.x == black_rings[i].x && c.y == black_rings[i].y)
				black_rings.erase(black_rings.begin() + i);
		}
	}
}

void Board::execute_move_place_ring(location l){
	int ring_no = turn_id*2, h = l.hexagon, p = l.position;
	placeRing(ring_no, location_to_coordinates(l));
}

void Board::execute_move_move_ring(location start, location end){
	moveRing(location_to_coordinates(start), location_to_coordinates(end));
}

void Board::execute_move_remove_row_ring(location start, location end, location ring){
	removeMarkerSeq(location_to_coordinates(start), location_to_coordinates(end));
	removeRing(location_to_coordinates(ring));
}

double Board::eval_func(){
	double hugeNumber = 100000;

	// if terminal return large number
	if ((turn_id==-1 && white_rings_out>=rings_to_remove)||(turn_id==1 && black_rings_out>=rings_to_remove)){
		return hugeNumber;
	}
	if ((turn_id==1 && white_rings_out>=rings_to_remove)||(turn_id==-1 && black_rings_out>=rings_to_remove)){
		return -hugeNumber;
	}

	double ring_weights = 10, marker_weights = 1;
	double score;
	if (turn_id==-1)
		score = ring_weights*(1.1*white_rings_out-black_rings_out) - marker_weights*(white_markers-black_markers);
	else
		score = ring_weights*(white_rings_out-1.1*black_rings_out) - marker_weights*(white_markers-black_markers);

	return score;
}

int Board::blocked_rings(coordinates c){
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
	
	//moving down
	it.x=c.x;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.y++;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	
	//moving right
	it.x=c.x+1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.x++;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	
	//moving left
	it.x=c.x-1;
	it.y=c.y;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
		it.x--;
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	
	// moving right down
	it.x=c.x+1;
	it.y=c.y+1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
	{	
		it.x++;it.y++; 
	}
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
	
	// moving left up
	it.x=c.x-1;
	it.y=c.y-1;
	while (checkValid(it) && abs(board[it.x][it.y])==1)
	{	
		it.x--;it.y--; 
	}
	if (checkValid(it) && board[it.x][it.y]!=0)
		retVal-=ring_id/board[it.x][it.y];
}

void Board::execute_move(string s){
	std::vector<std::string> tokens = split(s, ' ');
	if(tokens[0].compare("P") == 0){
		location l;
		l.hexagon = std::stoi(tokens[1]);
		l.position = std::stoi(tokens[2]);
		execute_move_place_ring(l);
		turn_id *= -1;
		return;
	}
	if(tokens[0].compare("S") == 0 && tokens[3].compare("M") == 0){
		location start,end;
		start.hexagon = std::stoi(tokens[1]);
		start.position = std::stoi(tokens[2]);
		end.hexagon = std::stoi(tokens[4]);
		end.position = std::stoi(tokens[5]);
		execute_move_move_ring(start, end);
		if(tokens.size() > 6){
			if(tokens[7].compare("RS") == 0 && tokens[10].compare("RE") == 0 && tokens[13].compare("X") == 0){
				location start,end,ring;
				start.hexagon = std::stoi(tokens[8]);
				start.position = std::stoi(tokens[9]);
				end.hexagon = std::stoi(tokens[11]);
				end.position = std::stoi(tokens[12]);
				ring.hexagon = std::stoi(tokens[14]);
				ring.position = std::stoi(tokens[15]);
				execute_move_remove_row_ring(start, end, ring);
			}
		}
	}
	turn_id *= -1;
}

vector<pair<Board*, string>> Board::get_valid_moves(){		//does not give children when all the rings have not been placed // use only for move ring and remove ring
	std::vector<pair<Board*, string>> children;
	string s = "";
	coordinates initial;
	coordinates final;
	bool flag = true;	
	location inital_location;
	int rings_in;
	if(turn_id == -1)
		rings_in = white_rings_in;
	else
		rings_in = black_rings_in;
	for(int i = 0; i < rings_in; i++){
		if(turn_id == -1)
			initial = white_rings[i];
		else
			initial = black_rings[i];

		
		//moving up
		final.x = initial.x; final.y = initial.y-1;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.y--;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.y--;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
		//moving down
		final.x = initial.x; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.y++;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.y++;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
		//moving along increasing x
		final.x = initial.x + 1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.x++;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.x++;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
		//moving along decreasing x
		final.x = initial.x-1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.x--;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.x--;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
		//moving along increasing z
		final.x = initial.x+1; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.x++;
			final.y++;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.x++;
			final.y++;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
		//moving along decreasing z
		final.x = initial.x-1; final.y = initial.y-1;
		flag = true;
		while (checkValid(final) && board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
			final.x--;
			final.y--;
		}
		while (checkValid(final) && board[final.x][final.y]!=0){
			if(board[final.x][final.y] == 2 || board[final.x][final.y] == -2)
				flag = false;
			final.x--;
			final.y--;
		}
		if(checkValid(final) && board[final.x][final.y] != -2 && board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(initial, final);
			children.push_back(p);
		}
	}
	return children;
	//selecting a ring
}

bool Board::check_terminal(){
	if(white_rings_out >= rings_to_remove || black_rings_out >= rings_to_remove)
		return true;
	return false;
}

Board* Board::copy_board(){
	Board *newBoard = new Board();
	newBoard->black_rings_in = this->black_rings_in;
	newBoard->black_rings_out = this->black_rings_out;
	newBoard->white_rings_in = this->white_rings_in;
	newBoard->white_rings_out = this->white_rings_out;
	newBoard->turn_id = this->turn_id;
	newBoard->current_score = this->current_score;
	for(int i = 0; i < this->white_rings.size(); i++)
		newBoard->white_rings.push_back(this->white_rings[i]);
	for(int i = 0; i < this->black_rings.size(); i++)
		newBoard->black_rings.push_back(this->black_rings[i]);
	for(int i = 0; i < 2*board_size+1; i++){
		for(int j = 0; j < 2*board_size+1; j++){
			newBoard->board[i][j] = this->board[i][j];
		}
	}
	return newBoard;
}

void Board::printBoard(){
	for(int i = 0; i < 2*board_size+1; i++){
		for(int j = 0; j < 2*board_size+1; j++){
			coordinates c;
			c.x = i;
			c.y = j;
			if (checkValid(c))
				cerr << board[i][j] << " ";
			else
				cerr << "  ";
			if(j == 2*board_size)
				cerr << endl;
		}
	}
}

void Board::iterate_over_line(int value, int c, int line_no, int start, int end){	// value = -1 for flip, 0 for remove //  c = 0 for x, 1 for y //
	for(int i = start; i <= end; i++){
		if(c == 0)
		{
			if(board[i][line_no] == 1)
				black_markers--;
			else if(board[i][line_no] == -1)
				white_markers--;
			board[i][line_no] *= value;
			if(board[i][line_no] == 1)
				black_markers++;
			else if(board[i][line_no] == -1)
				white_markers++;
		}
		else
		{
			if(board[line_no][i] == 1)
				black_markers--;
			else if(board[line_no][i] == -1)
				white_markers--;
			board[line_no][i] *= value;
			if(board[line_no][i] == 1)
				black_markers++;
			else if(board[line_no][i] == -1)
				white_markers++;		
		}
	}
}

coordinates Board::location_to_coordinates(location l){
	coordinates c;
	c.x = board_size;
	c.y = board_size;
	int h = l.hexagon, p = l.position;
	if (h == 0){
		return c;	
	}
	switch(p/h){
		case 0 : c.x += p; c.y += p-h; break;
		case 1 : c.x += h; c.y += p-h; break;
		case 2 : c.x += 3*h-p; c.y += h; break; 
		case 3 : c.x += 3*h-p; c.y += 4*h-p; break;
		case 4 : c.x += -1*h; c.y += 4*h-p; break;
		case 5 : c.x += p-6*h; c.y += -1*h; break;
		default : break;
	}
	return c;
}

location Board::coordinates_to_location(coordinates c){
	c.x-=board_size;
	c.y=board_size-c.y;

	location l;
	if (c.x>=0 && c.y>=0)
	{
		l.hexagon=abs(c.x+c.y);
		l.position=c.x;
	}
	else if (c.x>=0 && c.y<0)
	{
		l.hexagon=max(abs(c.x),abs(c.y));
		if (c.x==l.hexagon)
			l.position=l.hexagon-c.y;
		else
			l.position=3*l.hexagon-c.x;
	}
	else if (c.x<0 && c.y<0)
	{
		l.hexagon=abs(c.x+c.y);		
		l.position=3*l.hexagon-c.x;		
	}
	else
	{
		l.hexagon=max(abs(c.x),abs(c.y));
		if (c.x== -(l.hexagon))
			l.position=4*l.hexagon+c.y;
		else
			l.position=(6*l.hexagon+c.x)%(6*l.hexagon);
	}
	return l;
}

bool Board::checkValid(coordinates c){
	location l = coordinates_to_location(c);
	if(l.hexagon == 0 && l.position == 0)
		return true;
	if (l.hexagon==board_size)
		return (l.position<6*l.hexagon && l.position%l.hexagon);
	else
		return (l.hexagon<board_size && l.position<6*l.hexagon);
	return false;
}

vector<pair<coordinates, coordinates>> Board::get_markers_in_a_row(){
	vector<pair<coordinates,coordinates>> retVal;
	coordinates start_coord,end_coord;
	location start,end;
	for (int i=0;i<2*board_size+1;i++){
		for (int j=0;j<2*board_size+1;j++){
			start_coord.x=i;
			start_coord.y=j;
			if (!(checkValid(start_coord) && board[i][j]==turn_id))
				continue;
			
			//moving down
			cerr<<"check down"<<endl;
			int counter=1,k=j+1,l;
			end_coord.x=i;
			end_coord.y=j-1;
			if (!checkValid(end_coord) || board[i][j-1]!=turn_id){
				end_coord.y=k;
				while(checkValid(end_coord))
				{
					if (board[i][k]!=turn_id)
						break;
					counter++;
					end_coord.y=k++;
					if (counter==markers_in_line)
						break;
				}
			}
			
			if (counter==markers_in_line)
				retVal.push_back(make_pair(start_coord,end_coord));

			// moving up
			cerr<<"check up"<<endl;
			counter=1;
			k=j-1;
			end_coord.x=i;
			end_coord.y=j+1;
			if (!checkValid(end_coord) || board[i][j-1]!=turn_id){
				end_coord.y=k;
				while(checkValid(end_coord))
				{
					if (board[i][k]!=turn_id)
						break;
					counter++;
					end_coord.y=k--;
					if (counter==markers_in_line)
						break;
				}
			}
			if (counter==markers_in_line)
				retVal.push_back(make_pair(start_coord,end_coord));

			// moving right sideways
			cerr<<"check right"<<endl;
			counter=1;
			k=i+1;
			end_coord.x=i-1;
			end_coord.y=j;
			if (!checkValid(end_coord) || board[i-1][j]!=turn_id){
				end_coord.x=k;
				while(checkValid(end_coord))
				{
					if (board[k][j]!=turn_id)
						break;
					counter++;
					end_coord.x=k++;
					if (counter==markers_in_line)
						break;
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving left sideways
			cerr<<"check left"<<endl;
			counter=1;
			k=i-1;
			end_coord.x=i+1;
			end_coord.y=j;
			if (!checkValid(end_coord) || board[i+1][j]!=turn_id){
				end_coord.x=k;
				while(checkValid(end_coord))
				{
					if (board[k][j]!=turn_id)
						break;
					counter++;
					end_coord.x=k--;
					if (counter==markers_in_line)
						break;
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving diagonal right down
			cerr<<"check diagonal right"<<endl;
			counter=1;
			k=i+1;
			l=j+1;
			end_coord.x=i-1;
			end_coord.y=j-1;
			if (!checkValid(end_coord) || board[i-1][j-1]!=turn_id){
				end_coord.x=k;
				end_coord.y=l;
				cerr<<k<<" "<<l<<endl;
				while(checkValid(end_coord))
				{
					if (board[k][l]!=turn_id)
						break;
					counter++;
					end_coord.x=k++;
					end_coord.y=l++;
					if (counter==markers_in_line)
						break;
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving diagonal right down
			cerr<<"check diagonal leftt"<<endl;
			
			counter=1;
			k=i-1;
			l=j-1;
			end_coord.x=i+1;
			end_coord.y=j+1;
			if (!checkValid(end_coord) || board[i+1][j+1]!=turn_id){
				end_coord.x=k;
				end_coord.y=l;
				while(checkValid(end_coord))
				{
					if (board[k][l]!=turn_id)
						break;
					counter++;
					end_coord.x=k--;
					end_coord.y=l--;
					if (counter==markers_in_line)
						break;
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
		}
	}
	return retVal;
}

std::pair<Board*, string> Board::moveRing_to_pair(coordinates start, coordinates end){
	string s = "";
	location inital_location = coordinates_to_location(start);
	s += "S " + to_string(inital_location.hexagon) + " " + to_string(inital_location.position) + " ";
	location newLocation = coordinates_to_location(end);
	s += "M " + to_string(newLocation.hexagon) + " " + to_string(newLocation.position) + " ";
	Board* newBoard = this->copy_board();
	newBoard->moveRing(start, end);
	newBoard->turn_id *= -1;
	//check for five rings in a rows
	vector<pair<coordinates, coordinates>> v = get_markers_in_a_row();
	if(v.size() > 0){
		coordinates start = v[0].first;
		coordinates end = v[0].second;
		location initial = coordinates_to_location(start);
		location final = coordinates_to_location(end);
		newBoard->removeMarkerSeq(start, end);
		s += "RS " + to_string(initial.hexagon) + " " + to_string(initial.position) + " ";
		s += "RE " + to_string(final.hexagon) + " " + to_string(final.position) + " ";
		coordinates ring;
		if(turn_id == -1){
			ring = black_rings[0];
		}
		else{
			ring = white_rings[0];
		}
		removeRing(ring);
		location ring_l = coordinates_to_location(ring);
		s += "X " + to_string(ring_l.hexagon) + " " + to_string(ring_l.position) + " ";
	}
	//cout<<"*******************";
	//newBoard->printBoard();
	pair<Board*, string> p=make_pair(newBoard, s);
	return p;
}