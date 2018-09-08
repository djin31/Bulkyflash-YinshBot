#ifndef BOT_H
#define BOT_H

#include "treenode.h"
using namespace std;

class Bot{
  public:
    Treenode* root;
    int player_id;
    double time_limit;

    Bot(int player_id, double time);  

    void minVal(Treenode* node, double alpha, double beta, int depth_left);  

    void maxVal(Treenode* node, double alpha, double beta, int depth_left);

    void play();

    void read_move();

};
#endif