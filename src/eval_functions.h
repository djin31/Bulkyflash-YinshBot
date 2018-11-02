#ifndef EVAL_FUNC_H
#define EVAL_FUNC_H

#include "eval_functions.h"
#include "board.h"

static double RING_WEIGHTS = 10000;
static double MARKER_WEIGHTS = 2;
static double BLOCKING_WEIGHT= 5;
static double WEIGHT_MARKERS_IN_LINE= 0.5;
static double NORMALIZE_WEIGHT = 0.5;
static double WEIGHT_TO_RING_IN_LINE = 2.5;

void set_eval_parameters(vector<double> params); 
double eval_func(const Board &board, int player_id); //give the goodness of the current state of the board
int blocked_rings(const vector<vector<int>> &board, coordinates c); // gives the number of rings which are blocked
int self_blocked_rings(const vector<vector<int>> &board, coordinates c);
double eval_markers_in_row(const vector<vector<int>> &board, int board_size);	

#endif