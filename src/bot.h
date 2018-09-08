#ifndef BOT_H
#define BOT_H

#include "treenode.h"
using namespace std;

class Bot{
  Treenode* root;
  int player_id;

  Bot(int player_id);  

  void minVal(Treenode* node, double alpha, double beta, int depth_left);  

  void maxVal(Treenode* node, double alpha, double beta, int depth_left);

  void make_move();

  void place_rings();
};
#endif