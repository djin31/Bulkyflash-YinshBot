#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
using namespace std;

class Board{
	public:
		vector<vector<int>> board;
		int board_size;			// N
		int given_rings;		// M
		int rings_to_remove;	// L
		int markers_in_line;	// K
		int black_markers;
		int white_markers;
		int black_rings_out;
		int white_rings_out;
		int turn_id;

		vector<pair<int,int>> white_rings;
		vector<pair<int,int>> black_rings;

		Board(int board_size, int given_rings, int rings_to_remove, int markers_in_line);

		Board* execute_move();

		int eval_func();

		vector<Board*> get_valid_moves(int player_id);
}

#endif