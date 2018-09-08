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
	
	turn_id = 0;	// -1 << white, 1 << black

	// initialise board to all zeroes
	std::vector<int> zeroes;
	for(int i = 0; i < 2*board_size + 1; i++)
		zeroes.push_back(0);
	for (int i=1;i<=2*board_size + 1;i++)
		board.push_back(zeroes);
}

bool Board::placeRing(int ring_no, coordinates c){
	if(c.x > 2*board_size || c.y >= 2*board_size)
		return false;
	
	board[c.x][c.y] = ring_no;
	if (ring_no == 2)
		black_rings_in++;
	else
		black_rings_out++;
	
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
}

void Board::execute_move_place_ring(location l){
	int ring_no = turn_id*2, h = l.hexagon, p = l.position;
	placeRing(ring_no, location_to_coordinates(l));
}

void Board::iterate_over_line(int value, int c, int line_no, int start, int end){	// value = -1 for flip, 0 for remove //  c = 0 for x, 1 for y //
	for(int i = start; i <= end; i++){
		if(c == 0)
			board[i][line_no] *= value;
		else
			board[line_no][i] *= value;
	}
}

coordinates Board::location_to_coordinates(location l){
	coordinates c;
	int h = l.hexagon, p = l.position;
	switch(p/h){
		case 0 : c.x = p; break;
		case 1 : break;
		case 2 : break; 
		case 3 : break;
		case 4 : break;
		case 5 : break;
		default : break;
	}
	return c;
}