#include "board.h"

Board::Board(int board_size=5, int given_rings=5, int rings_to_remove=3, int markers_in_line=5){
	this->board_size=board_size;		
	this->given_rings=given_rings;		
	this->rings_to_remove=rings_to_remove;	
	this->markers_in_line=markers_in_line;

	black_markers=0;
	white_markers=0;
	black_rings_out=0;
	white_rings_out=0;

	// initialise board to all zeroes
	board = new vector<vector<int>>((board_size+1));
	board[0].push_back(0);
	for (int i=1;i<=board_size;i++)
	{
		for (int j=0;j<6*i;j++)
			board[i].push_back(0);
	}
}
