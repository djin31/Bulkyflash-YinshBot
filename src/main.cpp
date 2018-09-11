#include<iostream>
#include "bot.h"

using namespace std;

int main()
{
	int player_id, board_size, time_limit;
	//cin >> player_id >> board_size >> time_limit;
	//Bot bot = Bot(player_id, time_limit);
	//bot.play();
	Board b;
	b.turn_id*=-1;
	b.printBoard();
	b.execute_move("P 0 0");
	b.printBoard();
	//b.execute_move("P 1 0");
	//b.printBoard();
	b.execute_move("P 1 1");
	b.printBoard();
	// Board *b1 = b.copy_board();
	// b1->printBoard();
	// b.execute_move("S 1 1 M 2 2");
	// b.printBoard();
	std::vector<pair<Board*, string>> v = b.get_valid_moves();
	for(int i = 0; i < v.size(); i++){
		v[i].first->printBoard();
		cout<<v[i].second<<endl;
	}
	return 0;
}

