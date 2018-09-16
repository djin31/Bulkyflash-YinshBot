#ifndef BOT_H
#define BOT_H

#include "treenode.h"
#include <cstdlib>
#include <time.h> 

using namespace std;

class Bot{
  public:
    Treenode* root;
    int player_id;
    double time_left;
    Board* check_board;

    Bot(int player_id, double time);  

    void minVal(Treenode* node, double alpha, double beta, int depth_left);  

    void maxVal(Treenode* node, double alpha, double beta, int depth_left);

    location best_place_ring();
    void place_ring();

    void play();

    void read_move();

};
#endif