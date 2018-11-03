#include<iostream>
#include "bot.h"
#include "util.h"
using namespace std;

int board_size;

int main(int argc, char*argv[])
{
	int player_id, time_limit, seq_length=5, given_rings;
	string dummy;
	cin >> player_id >> board_size >> time_limit >> seq_length;
	getline(cin,dummy);
	given_rings = board_size;
	Board::set_board_params(board_size,given_rings,3,seq_length);
	
	vector<double> params;
	for (int i=1;i<argc;i++){
		params.push_back(stof(argv[i]));
	}
	set_eval_params(params);
	
	if (player_id==2)
		player_id=1;
	else
		player_id=-1;

	Bot bot = Bot(player_id,time_limit);
	bot.play();
	return 0;
}