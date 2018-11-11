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
	
	// MARKER_WEIGHTS, BLOCKING_WEIGHT, WEIGHT_MARKERS_IN_LINE, WEIGHT_TO_RING_IN_LINE, CONTROL_MARKERS, RH_WEIGHT
	double params55[6] = {1.5, 2.0, 2.5, 3.0, 0.5 , 0.5};
	double params65[6] = {1.5, 2.0, 2.5, 3.0, 0.5 , 0.5};
	double params66[6] = {1.5, 2.0, 2.5, 3.0, 0.5 , 0.5};

	vector<double> params;
	
	// for (int i=0;i<argc;i++){
	// 	params.push_back(stof(argv[i]));
	// }

	if (board_size==5){
		params = vector<double>(begin(params55), end(params55));
	}
	else{
		if (seq_length==5)
			params = vector<double>(begin(params65), end(params65));
		else
			params = vector<double>(begin(params66), end(params66));
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