#ifndef BOT_H
#define BOT_H

#include <cstdlib>
#include <time.h> 
#include "board.h"
#include "eval_functions.h"

using namespace std;

extern int board_size;

class Bot{
  public:
    int player_id;
    double time_left;
    Board* board;

    Bot(int player_id, double time);  

    double minVal(Board *board, double alpha, double beta, int depth_left);
    double maxVal(Board *board, double alpha, double beta, int depth_left);

    location best_place_ring();
    void place_ring();
    void minimax_decision(int MOVE_NO);
    void play();
    void read_move();
};
#endif