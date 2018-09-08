#include "board.h"

Board::Board(){

	black_markers=0;
	white_markers=0;
	black_rings_out=0;
	white_rings_out=0;

	// initialise board to all zeroes
	board = vector<vector<int>>((board_size+1));
	board[0].push_back(0);
	for (int i=1;i<=BOARD_SIZE;i++)
	{
		for (int j=0;j<6*i;j++)
			board[i].push_back(0);
	}
}

bool Board::placeRing(int ring_no, location l){
	if(l.hexagon > board_size || l.position >= 6*hexagon)
		return false;
	board[l.hexagon][l.position] = ring_no;
	return true;
}

bool Board::moveRing(int ring_no, location start, location end){
	board[start.hexagon][start.position] = ring_no/2;
	board[end.hexagon][end.position] = ring_no;
	int h1 = start.hexagon;
	int p1 = start.position;
	int h2 = end.hexagon;
	int p2 = end.position;
}
bool removeMarkerSeq(location start, location end);
bool removeRing(location l);