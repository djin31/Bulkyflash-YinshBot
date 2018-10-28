#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include "util.h"


using namespace std;
		
struct location
{
	int hexagon;
	int position;

};

struct coordinates
{
	int x;	//vertical line
	int y;	//line 60* to the horizontal (CCW)
};


class Board{
	public:
		vector<vector<int>> board;		// 0 -> empty, 1 -> black marker, 2 -> black ring, -1 -> white marker, -2 -> white ring
		
		int black_markers;
		int white_markers;
		int black_rings_in;
		int white_rings_in;
		int black_rings_out;
		int white_rings_out;
		
		int turn_id;	// -1 << white, 1 << black	

		vector<coordinates> white_rings;
		vector<coordinates> black_rings;

		//helper functions
		void iterate_over_line(int value, int c, int line_no, int start, int end);	// value = -1 for flip, 0 for remove //  c = 0 for iterating over x, 1 for y // iteration only along x or y
		coordinates location_to_coordinates(location l);
		location coordinates_to_location(coordinates c);
		std::pair<Board*, string> moveRing_to_pair(Board *b, coordinates start, coordinates end, string removeMarker_string);
		string moveRing_to_string(coordinates start, coordinates end, string removeMarker_string);

		// ring_no : black ring << 2, white ring << -2
		// These function do not check the validity of the move, they can only tell if the location is outside the board
		bool placeRing(int ring_no, coordinates c);
		bool moveRing(coordinates start, coordinates end);
		bool removeMarkerSeq(coordinates start, coordinates end);
		bool removeRing(coordinates c);
		void execute_move_place_ring(location l);
		void execute_move_move_ring(location start, location end);
		void execute_move_remove_row_ring(location start, location end, location ring);
		vector<pair<coordinates, coordinates>> get_markers_in_a_row();
		vector<pair<Board*, string>> possible_removeMarker_orders(Board* b);
		vector<pair<Board*, string>> possible_moveRing_orders(Board* b, string removeMarker_string);
		vector<string> possible_removeMarker_actions(Board* b);
		vector<string> possible_moveRing_actions(Board* b, string removeMarker_string);
		bool checkValid(coordinates c);
	//=========== use ony these functions publically===========
		Board();
		
		//execute a move on the board // s has space after every character
		void execute_move(string s);

		//give the goodness of the current state of the board
		double eval_func(int player_id);

		// gives the number of rings which are blocked
		int blocked_rings(coordinates c);
		int self_blocked_rings(coordinates c);
		double eval_markers_in_row();

		//get the valid moves on the current board
		vector<pair<Board*, string>> get_valid_moves();		//does not give children when all the rings have not been placed // use only for move ring and remove ring
		vector<string> get_valid_actions();

		//check if L rings have been removed
		bool check_terminal();

		//function to copy the board
		Board* copy_board();

		//function to print the board
		void printBoard();
		
		
	static int board_size;			// N
	static int given_rings;		// M
	static int rings_to_remove;	// L
	static int markers_in_line;	// K

	static void set_board_params(int board_size_in, int given_rings_in, int rings_to_remove_in, int markers_in_line_in);


};

#endif