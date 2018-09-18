#include<iostream>
#include "bot.h"

using namespace std;

int main()
{
	int player_id, board_size, time_limit;
	cin >> player_id >> board_size >> time_limit;
	string dummy;
	getline(cin,dummy);
	if (player_id==2)
		player_id=1;
	else
		player_id=-1;
	Bot bot = Bot(player_id,time_limit);
	bot.play();
	return 0;
}

