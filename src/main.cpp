#include<iostream>
#include "bot.h"

using namespace std;

int main()
{
	int player_id, board_size, time_limit;
	cin >> player_id >> board_size >> time_limit;
	Bot bot = Bot(player_id, time_limit);
	bot.play();
	return 0;
}

