#include "bot.h"

Bot::Bot(int player_id){
	root = new Treenode();
	// This is done since could not initialise Board in constructor 
	// to avoid multiple board_copies during set_children()
	root->board = new Board();  
	this->player_id = player_id;
}