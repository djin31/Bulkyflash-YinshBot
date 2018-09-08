#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
using namespace std;

struct location
{
	int hexagon;
	int position;
};

class Board{
	private:
		vector<vector<int>> board;		// 0 -> empty, 1 -> black marker, 2 -> black ring, -1 -> white marker, -2 -> white ring
		int board_size;			// N
		int given_rings;		// M
		int rings_to_remove;	// L
		int markers_in_line;	// K
		int black_markers;
		int white_markers;
		int black_rings_in;
		int white_rings_in;
		int black_rings_out;
		int white_rings_out;
		int turn_id;

		vector<pair<int,int>> white_rings;
		vector<pair<int,int>> black_rings;

		// ring_no : black ring << 2, white ring << -2
		// These function do not check the validity of the move, they can only tell if the location is outside the board
		bool placeRing(int ring_no, location l);
		bool moveRing(location start, location end);
		bool removeMarkerSeq(location start, location end);
		bool removeRing(location l);

	public:
		Board(int board_size, int given_rings, int rings_to_remove, int markers_in_line);
		Board* execute_move();

		int eval_func();

		vector<Board*> get_valid_moves(int player_id);
};

#endif