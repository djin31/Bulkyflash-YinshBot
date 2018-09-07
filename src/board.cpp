#include "board.h"

Board::Board(){

	black_markers=0;
	white_markers=0;
	black_rings_out=0;
	white_rings_out=0;

	// initialise board to all zeroes
	board = new vector<vector<int>>((BOARD_SIZE+1));
	board[0].push_back(0);
	for (int i=1;i<=BOARD_SIZE;i++)
	{
		for (int j=0;j<6*i;j++)
			board[i].push_back(0);
	}
}
