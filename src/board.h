#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;

template<typename Out>

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
		
		int turn_id;	// -1 << white, 1 << black			

		vector<coordinates> white_rings;
		vector<coordinates> black_rings;

		//helper functions
		void iterate_over_line(int value, int c, int line_no, int start, int end);	// value = -1 for flip, 0 for remove //  c = 0 for iterating over x, 1 for y // iteration only along x or y
		coordinates location_to_coordinates(location l);
		location coordinates_to_location(coordinates c);
		void split(const std::string &s, char delim, Out result);
		std::vector<std::string> split(const std::string &s, char delim);

		// ring_no : black ring << 2, white ring << -2
		// These function do not check the validity of the move, they can only tell if the location is outside the board
		bool placeRing(int ring_no, coordinates c);
		bool moveRing(coordinates start, coordinates end);
		bool removeMarkerSeq(coordinates start, coordinates end);
		bool removeRing(coordinates c);
		void execute_move_place_ring(location l);
		void execute_move_move_ring(location start, location end);
		void execute_move_remove_row_ring(location start, location end, location ring);

	public:
		Board(int board_size, int given_rings, int rings_to_remove, int markers_in_line);
		
		//execute a move on the board // s has space after every character
		void execute_move(string s);

		//give the goodness of the current state of the board
		double eval_func();

		//get the valid moves on the current board
		vector<pair<Board*, string>> get_valid_moves();

		//check if L rings have been removed
		bool check_terminal();
};

#endif