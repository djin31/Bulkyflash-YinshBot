#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
using namespace std;

int BOARD_SIZE;			// N
int GIVEN_RINGS;		// M
int RINGS_TO_REMOVE;	// L
int MARKERS_IN_LINE;	// K

class Board{
	public:
		vector<vector<int>> board;

		int black_markers;
		int white_markers;
		int black_rings_out;
		int white_rings_out;
		int turn_id;			

		vector<pair<int,int>> white_rings;
		vector<pair<int,int>> black_rings;

		Board();

		Board* execute_move();

		double eval_func();

		vector<pair<Board*, string>> get_valid_moves();

		bool check_terminal();
};

#endif