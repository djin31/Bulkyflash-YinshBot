#include "board.h"

Board::Board(int board_size, int given_rings, int rings_to_remove, int markers_in_line){

	vector<vector<int>> board;		// 0 -> empty, 1 -> black marker, 2 -> black ring, -1 -> white marker, -2 -> white ring
	this->board_size = board_size;			// N
	this->given_rings = given_rings;		// M
	this->rings_to_remove = rings_to_remove;	// L
	this->markers_in_line = markers_in_line;	// K
	
	black_rings_in = 0; 
	white_rings_in = 0;
	black_rings_out = 0;
	white_rings_out = 0;
	
	turn_id = -1;	// -1 << white, 1 << black
	currentScore = 0.0;

	// initialise board to all zeroes
	std::vector<int> zeroes;
	for(int i = 0; i < 2*board_size + 1; i++)
		zeroes.push_back(0);
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
			for(int i=  1; i < end.x - start.x; i++)
				board[start.x + i][start.y + i] *= -1;
		}
		else{
			for(int i=  1; i < start.x - end.x; i++)
				board[end.x + i][end.y + i] *= -1;	
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
			for(int i=  0; i <= end.x - start.x; i++)
				board[start.x + i][start.y + i] = 0;
		}
		else{
			for(int i=  0; i <= start.x - end.x; i++)
				board[end.x + i][end.y + i] = 0;	
		}
		return true;	
	}
	return false;
}

bool Board::removeRing(coordinates c){
	board[c.x][c.y] = 0;
	if (turn_id == -1){
		white_rings_in--; white_rings_out++;
	}
	else{
		black_rings_in--; black_rings_out++;
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
	double score = 0.0;
	score += 10.0*(white_rings_out-black_rings_out) - 2.0*(white_ma-black_rings_in);
	return turn_id*score;
}

void Board::execute_move(string s){
	std::vector<std::string> tokens = split(s, ' ');
	if(tokens[0].compare("P") == 0){
		location l;
		l.hexagon = std::stoi(tokens[1]);
		l.position = std::stoi(tokens[2]);
		execute_move_place_ring(l);
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
	location inital_location;
	if (turn_id == -1){
		for(int i = 0; i < white_rings_in; i++){
			initial = white_rings[i];

			//moving up
			coordinates final;
			final.x = initial.x; final.y = initial.y-1;
			bool flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.y--;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.y--;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
			//moving down
			coordinates final;
			final.x = initial.x; final.y = initial.y+1;
			bool flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.y++;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.y++;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
			//moving along increasing x
			coordinates final;
			final.x = initial.x + 1; final.y = initial.y;
			bool flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.x++;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.x++;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
			//moving along decreasing x
			coordinates final;
			final.x = initial.x-1; final.y = initial.y;
			bool flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.x--;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.x--;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
			//moving along increasing z
			coordinates final;
			final.x = initial.x+1; final.y = initial.y+1;
			bool  flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.x++;
				final.y++;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.x++;
				final.y++;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
			//moving along decreasing z
			coordinates final;
			final.x = initial.x-1; final.y = initial.y-1;
			bool flag = true;
			while (checkValid(final) && board[x][y]==0)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
				final.x--;
				final.y--;
			}
			while (checkValid(final) && board[x][y]!=0){
				if(board[x][y] == 2 || board[x][y] == -2)
					flag = false;
				final.x--;
				final.y--;
			}
			if(checkValid(final) && board[x][y] != -2 && board[x][y] != 2 && flag)
			{	
				pair<Board*, string> p = moveRing_to_pair(initial, final);
				children.push_back(p);
			}
		}
	}
	//selecting a ring
}

bool Board::check_terminal(){
	if(white_rings_out >= rings_to_remove || black_rings_out >= rings_to_remove)
		return true;
	return false;
}

Board* Board::copy_board(){
	Board *newBoard = new Board(board_size, given_rings, rings_to_remove, markers_in_line);
	newBoard->black_rings_in = this->black_rings_in;
	newBoard->black_rings_out = this->black_rings_out;
	newBoard->white_rings_in = this->white_rings_in;
	newBoard->white_rings_out = this->white_rings_out;
	newBoard->turn_id = this->turn_id;
	newBoard->currentScore = this->currentScore;
	for(int i = 0; i < this->white_rings.size(); i++)
		newBoard->white_rings.push_back(this->white_rings[i]);
	for(int i = 0; i < this->black_rings.size(); i++)
		newBoard->black_rings.push_back(this->black_rings[i]);
	for(int i = 0; i < 2*board_size+1; i++){
		std::vector<int> v;
		for(int j = 0; j < 2*board_size+1; j++){
			v.push_back(this->board[i][j]);
		}
		newBoard->board.push_back(v);
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
			if(j == 2*board_sizea)
				cerr << endl;
		}
	}
}

void Board::iterate_over_line(int value, int c, int line_no, int start, int end){	// value = -1 for flip, 0 for remove //  c = 0 for x, 1 for y //
	for(int i = start; i <= end; i++){
		if(c == 0)
		{
			board[i][line_no] *= value;
		}
		else
		{
			board[line_no][i] *= value;
		}
	}
}

coordinates Board::location_to_coordinates(location l){
	coordinates c;
	c.x = board_size;
	c.y = board_size;
	int h = l.hexagon, p = l.position;
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
		l.position=c.y;
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
			l.position=(6*l.hexagon+c.x)%l.hexagon;
	}
	return l;
}

void Board::split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> Board::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::pair<Board*, string> moveRing_to_pair(coordinates start, coordinates end){
	string s = "";
	location inital_location = coordinates_to_location(start);
	s += "S " + to_string(inital_location.hexagon) + " " + to_string(inital_location.position) + " ";
	location newLocation = coordinates_to_location(end);
	s += "M " + to_string(newLocation.hexagon) + " " + to_string(newLocation.position) + " ";
	Board* newBoard = this->copy_board();
	newBoard->moveRing(start, end);
	//check for five rings in a rows
	pair<Board*, string> p(newBoard, s);
	return p;
}