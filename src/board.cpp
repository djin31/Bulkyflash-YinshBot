#include "board.h"

Board::Board(){

	black_markers = 0;
	white_markers = 0;
	black_rings_in = 0; 
	white_rings_in = 0;
	black_rings_out = 0;
	white_rings_out = 0;
	
	turn_id = -1;	// -1 << white, 1 << black

	// initialise board to all zeroes
	vector<int> zeroes(2*board_size+1,0);
	
	for (int i=0;i<2*board_size + 1;i++)
		board.push_back(zeroes);
}

bool Board::placeRing(int ring_no, coordinates c){
	
	board[c.x][c.y] = ring_no;
	
	if (ring_no == 2){
		black_rings_in++; black_rings.push_back(c);
	}
	else{
		white_rings_in++; white_rings.push_back(c);
	}
	
	return true;
}

bool Board::moveRing(coordinates start, coordinates end){
	int ring_no = board[start.x][start.y];
	board[start.x][start.y] = ring_no/2;
	board[end.x][end.y] = ring_no;
	
	//changing ring vectors
	if(ring_no == 2){
		for(int i = 0; i < black_rings.size(); i++){
			if(black_rings[i].x == start.x && black_rings[i].y == start.y){
				black_rings[i].x = end.x;
				black_rings[i].y = end.y;
				break;
			}
		}
	}
	else{
		for(int i = 0; i < white_rings.size(); i++){
			if(white_rings[i].x == start.x && white_rings[i].y == start.y){
				white_rings[i].x = end.x;
				white_rings[i].y = end.y;
				break;
			}
		}
	}

	//incrementing number of markers
	if(ring_no == 2)
		black_markers++;
	else
		white_markers++;

	//flipping markers
	if(start.x == end.x){
		if(start.y <= end.y)
			iterate_over_line(-1, 1, start.x, start.y+1, end.y-1);
		else
			iterate_over_line(-1, 1, start.x, end.y+1, start.y-1);
		return true;
	}	
	if(start.y == end.y){
		if(start.x <= end.x)
			iterate_over_line(-1, 0, start.y, start.x+1, end.x-1);
		else
			iterate_over_line(-1, 0, start.y, end.x+1, start.x-1);
		return true;
	}
	if(start.x - start.y == end.x - end.y){
		if(start.x <= end.x){
			for(int i=  1; i < end.x - start.x; i++){
				if(board[start.x + i][start.y + i] == 1)
					black_markers--;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers--;
				board[start.x + i][start.y + i] *= -1;
				if(board[start.x + i][start.y + i] == 1)
					black_markers++;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers++;
			}
		}
		else{
			for(int i=  1; i < start.x - end.x; i++){
				if(board[end.x + i][end.y + i] == 1)
					black_markers--;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers--;
				board[end.x + i][end.y + i] *= -1;	
				if(board[end.x + i][end.y + i] == 1)
					black_markers++;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers++;
			}
		}
		return true;	
	}
	return false;
}

bool Board::removeMarkerSeq(coordinates start, coordinates end){
	if(start.x == end.x){
		if(start.y <= end.y)
			iterate_over_line(0, 1, start.x, start.y, end.y);
		else
			iterate_over_line(0, 1, start.x, end.y, start.y);
		return true;
	}	
	if(start.y == end.y){
		if(start.x <= end.x)
			iterate_over_line(0, 0, start.y, start.x, end.x);
		else
			iterate_over_line(0, 0, start.y, end.x, start.x);
		return true;
	}
	if(start.x - start.y == end.x - end.y){
		if(start.x <= end.x){
			for(int i=  0; i <= end.x - start.x; i++){
				if(board[start.x + i][start.y + i] == 1)
					black_markers--;
				else if(board[start.x + i][start.y + i] == -1)
					white_markers--;
				board[start.x + i][start.y + i] = 0;
			}
		}
		else{
			for(int i=  0; i <= start.x - end.x; i++){
				if(board[end.x + i][end.y + i] == 1)
					black_markers--;
				else if(board[end.x + i][end.y + i] == -1)
					white_markers--;
				board[end.x + i][end.y + i] = 0;	
			}
		}
		return true;	
	}
	return false;
}

bool Board::removeRing(coordinates c){
	board[c.x][c.y] = 0;

	//updating ring vector
	if (turn_id == -1){
		white_rings_in--; white_rings_out++;
		for(int i = 0; i < white_rings.size(); i++){
			if(c.x == white_rings[i].x && c.y == white_rings[i].y)
				white_rings.erase(white_rings.begin() + i);
		}
	}
	else{
		black_rings_in--; black_rings_out++;
		for(int i = 0; i < black_rings.size(); i++){
			if(c.x == black_rings[i].x && c.y == black_rings[i].y)
				black_rings.erase(black_rings.begin() + i);
		}
	}
}

void Board::execute_move(string s){
	std::vector<std::string> tokens = split(s, ' ');
	int i = 0;
	//cerr<<"EXECUTE MOVE "<<tokens.size()<<endl;
	while(i < tokens.size()){
		if(tokens[i].compare("P") == 0){
			location l;
			l.hexagon = std::stoi(tokens[i+1]);
			l.position = std::stoi(tokens[i+2]);
			placeRing(turn_id*2, location_to_coordinates(l));
			turn_id *= -1;
			return;
		}
		else if(tokens[i].compare("S") == 0 && tokens[i+3].compare("M") == 0){
			location start,end;
			start.hexagon = std::stoi(tokens[i+1]);
			start.position = std::stoi(tokens[i+2]);
			end.hexagon = std::stoi(tokens[i+4]);
			end.position = std::stoi(tokens[i+5]);
			moveRing(location_to_coordinates(start), location_to_coordinates(end));
			i += 6;
		}
		else if(tokens[i].compare("RS") == 0 && tokens[i+3].compare("RE") == 0 && tokens[i+6].compare("X") == 0){
			location start,end,ring;
			start.hexagon = std::stoi(tokens[i+1]);
			start.position = std::stoi(tokens[i+2]);
			end.hexagon = std::stoi(tokens[i+4]);
			end.position = std::stoi(tokens[i+5]);
			ring.hexagon = std::stoi(tokens[i+7]);
			ring.position = std::stoi(tokens[i+8]);
			removeMarkerSeq(location_to_coordinates(start), location_to_coordinates(end));
			removeRing(location_to_coordinates(ring));
			i += 9;
		}
		else
			break;
	}
	
	turn_id *= -1;
}

vector<pair<Board*, string>> Board::get_valid_moves(){
	vector<pair<Board*, string>> possible_orders;
	vector<pair<Board*, string>> boards_after_removeMarker = possible_removeMarker_orders(this);
	vector<pair<Board*, string>> boards_after_moveRing;
	vector<pair<Board*, string>> boards_after_moveRing_removeMarker;
	
	//moving ring
	if (boards_after_removeMarker.size() == 0)
		boards_after_moveRing = possible_moveRing_orders(this, "");
	else{
		for(int i = 0; i < boards_after_removeMarker.size(); i++){
			if(!(boards_after_removeMarker[i].first->white_rings_out >= rings_to_remove || boards_after_removeMarker[i].first->black_rings_out >= rings_to_remove)){
				vector<pair<Board*, string>> v = possible_moveRing_orders(boards_after_removeMarker[i].first, boards_after_removeMarker[i].second);
				boards_after_moveRing.insert(boards_after_moveRing.end(), v.begin(), v.end());
			}
			else{
				boards_after_moveRing.push_back(boards_after_removeMarker[i]);	
			}
		}
	}

	//checking for five markers in a row after moveRing
	for(int i = 0; i < boards_after_moveRing.size(); i++){
		boards_after_moveRing[i].first->turn_id *= -1;
		vector<pair<Board*, string>> v; 
		if(!(boards_after_moveRing[i].first->white_rings_out >= rings_to_remove || boards_after_moveRing[i].first->black_rings_out >= rings_to_remove))
			v = possible_removeMarker_orders(boards_after_moveRing[i].first);
		if(v.size() == 0){
			boards_after_moveRing[i].first->turn_id*=-1;
			boards_after_moveRing_removeMarker.push_back(boards_after_moveRing[i]);
		}
		else{
			//cerr<<(boards_after_moveRing[i].second + v[0].second)<<"     INSIDE REMOVE MARKER\n";
			
			for(int j = 0; j < v.size(); j++){

				pair<Board*, string> p = make_pair(v[j].first, boards_after_moveRing[i].second + v[j].second);
				p.first->turn_id*=-1;
				boards_after_moveRing_removeMarker.push_back(p);
			}
		}
	}
	return boards_after_moveRing_removeMarker;
}

vector<string> Board::get_valid_actions(){
	vector<string> possible_orders;
	vector<string> boards_after_removeMarker = possible_removeMarker_actions(this);
	vector<string> boards_after_moveRing;
	vector<string> boards_after_moveRing_removeMarker;
	
	//moving ring
	if (boards_after_removeMarker.size() == 0)
		boards_after_moveRing = possible_moveRing_actions(this, "");
	else{
		for(int i = 0; i < boards_after_removeMarker.size(); i++){
			Board* temp_board_after_removeMarker = this->copy_board();
			temp_board_after_removeMarker->execute_move(boards_after_removeMarker[i]);
			if(!(temp_board_after_removeMarker->white_rings_out >= rings_to_remove || temp_board_after_removeMarker->black_rings_out >= rings_to_remove)){
				vector<string> v = possible_moveRing_actions(temp_board_after_removeMarker, boards_after_removeMarker[i]);
				boards_after_moveRing.insert(boards_after_moveRing.end(), v.begin(), v.end());
			}
			else{
				boards_after_moveRing.push_back(boards_after_removeMarker[i]);	
			}
			delete temp_board_after_removeMarker;
		}
	}

	//checking for five markers in a row after moveRing
	for(int i = 0; i < boards_after_moveRing.size(); i++){
		Board* temp_board_after_moveRing = this->copy_board();
		temp_board_after_moveRing->execute_move(boards_after_moveRing[i]);
		temp_board_after_moveRing->turn_id *= -1;
		vector<string> v; 
		if(!(temp_board_after_moveRing->white_rings_out >= rings_to_remove || temp_board_after_moveRing->black_rings_out >= rings_to_remove))
			v = possible_removeMarker_actions(temp_board_after_moveRing);
		if(v.size() == 0){
			boards_after_moveRing_removeMarker.push_back(boards_after_moveRing[i]);
		}
		else{
			for(int j = 0; j < v.size(); j++){
				string p = boards_after_moveRing[i] + v[j];
				boards_after_moveRing_removeMarker.push_back(p);
			}
		}
		delete temp_board_after_moveRing;
	}
	return boards_after_moveRing_removeMarker;
}

vector<pair<Board*, string>> Board::possible_moveRing_orders(Board* b, string removeMarker_string){		//does not give children when all the rings have not been placed // use only for move ring and remove ring
	std::vector<pair<Board*, string>> children;
	string s = "";
	coordinates initial;
	coordinates final;
	bool flag = true;	
	location inital_location;
	int rings_in;
	if(b->turn_id == -1)
		rings_in = b->white_rings_in;
	else
		rings_in = b->black_rings_in;
	for(int i = 0; i < rings_in; i++){
		if(b->turn_id == -1)
			initial = b->white_rings[i];
		else
			initial = b->black_rings[i];

		
		//moving up
		final.x = initial.x; final.y = initial.y-1;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.y--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.y--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving down
		final.x = initial.x; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.y++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.y++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along increasing x
		final.x = initial.x + 1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.x++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along decreasing x
		final.x = initial.x-1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.x--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along increasing z
		final.x = initial.x+1; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.x++;
			final.y++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x++;
			final.y++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along decreasing z
		final.x = initial.x-1; final.y = initial.y-1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
			final.x--;
			final.y--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x--;
			final.y--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			pair<Board*, string> p = moveRing_to_pair(b, initial, final, removeMarker_string);
			children.push_back(p);
		}
	}
	return children;
}

vector<string> Board::possible_moveRing_actions(Board* b, string removeMarker_string){		//does not give children when all the rings have not been placed // use only for move ring and remove ring
	vector<string> children;
	string s = "";
	coordinates initial;
	coordinates final;
	bool flag = true;	
	location inital_location;
	int rings_in;
	if(b->turn_id == -1)
		rings_in = b->white_rings_in;
	else
		rings_in = b->black_rings_in;
	for(int i = 0; i < rings_in; i++){
		if(b->turn_id == -1)
			initial = b->white_rings[i];
		else
			initial = b->black_rings[i];

		
		//moving up
		final.x = initial.x; final.y = initial.y-1;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.y--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.y--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string( initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving down
		final.x = initial.x; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.y++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.y++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along increasing x
		final.x = initial.x + 1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.x++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along decreasing x
		final.x = initial.x-1; final.y = initial.y;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.x--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along increasing z
		final.x = initial.x+1; final.y = initial.y+1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.x++;
			final.y++;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x++;
			final.y++;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
		}
		//moving along decreasing z
		final.x = initial.x-1; final.y = initial.y-1;
		flag = true;
		while (checkValid(final) && b->board[final.x][final.y]==0)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
			final.x--;
			final.y--;
		}
		while (checkValid(final) && b->board[final.x][final.y]!=0){
			if(b->board[final.x][final.y] == 2 || b->board[final.x][final.y] == -2)
				flag = false;
			final.x--;
			final.y--;
		}
		if(checkValid(final) && b->board[final.x][final.y] != -2 && b->board[final.x][final.y] != 2 && flag)
		{	
			string p = moveRing_to_string(initial, final, removeMarker_string);
			children.push_back(p);
		}
	}
	return children;
}

vector<pair<Board*, string>> Board::possible_removeMarker_orders(Board* b){
	vector<pair<Board*, string>> possible_orders;
	vector<pair<coordinates, coordinates>> fiveMarkerSeq = b->get_markers_in_a_row();
	for(int i = 0; i < fiveMarkerSeq.size(); i++){
		Board *newBoard = b->copy_board();
		pair<coordinates, coordinates> p = fiveMarkerSeq[i];
		location initial = coordinates_to_location(p.first);
		location final = coordinates_to_location(p.second);
		newBoard->removeMarkerSeq(p.first, p.second);
		string s = "";
		s += "RS " + to_string(initial.hexagon) + " " + to_string(initial.position) + " ";
		s += "RE " + to_string(final.hexagon) + " " + to_string(final.position) + " ";

		//remove ring
		coordinates ring;
		if(newBoard->turn_id == 1){
			for(int j = 0; j < newBoard->black_rings.size(); j++){
				ring = newBoard->black_rings[j];
				Board* tempNewBoard = newBoard->copy_board();
				tempNewBoard->removeRing(ring);
				location ring_l = coordinates_to_location(ring);
				string s1 = s + "X " + to_string(ring_l.hexagon) + " " + to_string(ring_l.position) + " ";

				vector<pair<Board*, string>> more_possibilities;
				if(!(tempNewBoard->white_rings_out >= rings_to_remove || tempNewBoard->black_rings_out >= rings_to_remove))
					more_possibilities = possible_removeMarker_orders(tempNewBoard);
				if(more_possibilities.size() == 0)
					possible_orders.push_back(make_pair(tempNewBoard, s1));
				else{
					delete tempNewBoard;
					for(int k = 0; k < more_possibilities.size(); k++){
						string s2 = s1 + more_possibilities[k].second;
						possible_orders.push_back(make_pair(more_possibilities[k].first, s2));
					}
				}
			}
			delete newBoard;
		}
		else{
			for(int j = 0; j < newBoard->white_rings.size(); j++){
				ring = newBoard->white_rings[j];
				Board* tempNewBoard = newBoard->copy_board();
				tempNewBoard->removeRing(ring);
				location ring_l = coordinates_to_location(ring);
				string s1 = s + "X " + to_string(ring_l.hexagon) + " " + to_string(ring_l.position) + " ";

				vector<pair<Board*, string>> more_possibilities;
				if(!(tempNewBoard->white_rings_out >= rings_to_remove || tempNewBoard->black_rings_out >= rings_to_remove))
					more_possibilities = possible_removeMarker_orders(tempNewBoard);
				if(more_possibilities.size() == 0)
					possible_orders.push_back(make_pair(tempNewBoard, s1));
				else{
					delete tempNewBoard;
					for(int k = 0; k < more_possibilities.size(); k++){
						string s2 = s1 + more_possibilities[k].second;
						possible_orders.push_back(make_pair(more_possibilities[k].first, s2));
					}
				}
			}
			delete newBoard;
		}
		
	}
	return possible_orders;
}

vector<string> Board::possible_removeMarker_actions(Board* b){
	vector<string> possible_orders;
	vector<pair<coordinates, coordinates>> fiveMarkerSeq = b->get_markers_in_a_row();
	for(int i = 0; i < fiveMarkerSeq.size(); i++){
		Board *newBoard = b->copy_board();
		pair<coordinates, coordinates> p = fiveMarkerSeq[i];
		location initial = coordinates_to_location(p.first);
		location final = coordinates_to_location(p.second);
		newBoard->removeMarkerSeq(p.first, p.second);
		string s = "";
		s += "RS " + to_string(initial.hexagon) + " " + to_string(initial.position) + " ";
		s += "RE " + to_string(final.hexagon) + " " + to_string(final.position) + " ";

		//remove ring
		coordinates ring;
		if(newBoard->turn_id == 1){
			for(int j = 0; j < newBoard->black_rings.size(); j++){
				ring = newBoard->black_rings[j];
				Board* tempNewBoard = newBoard->copy_board();
				tempNewBoard->removeRing(ring);
				location ring_l = coordinates_to_location(ring);
				string s1 = s + "X " + to_string(ring_l.hexagon) + " " + to_string(ring_l.position) + " ";

				vector<string> more_possibilities;
				if(!(tempNewBoard->white_rings_out >= rings_to_remove || tempNewBoard->black_rings_out >= rings_to_remove))
					more_possibilities = possible_removeMarker_actions(tempNewBoard);
				if(more_possibilities.size() == 0)
					possible_orders.push_back(s1);
				else{
					for(int k = 0; k < more_possibilities.size(); k++){
						string s2 = s1 + more_possibilities[k];
						possible_orders.push_back(s2);
					}
				}
				delete tempNewBoard;
			}
		}
		else{
			for(int j = 0; j < newBoard->white_rings.size(); j++){
				ring = newBoard->white_rings[j];
				Board* tempNewBoard = newBoard->copy_board();
				tempNewBoard->removeRing(ring);
				location ring_l = coordinates_to_location(ring);
				string s1 = s + "X " + to_string(ring_l.hexagon) + " " + to_string(ring_l.position) + " ";

				vector<string> more_possibilities;
				if(!(tempNewBoard->white_rings_out >= rings_to_remove || tempNewBoard->black_rings_out >= rings_to_remove))
					more_possibilities = possible_removeMarker_actions(tempNewBoard);
				if(more_possibilities.size() == 0)
					possible_orders.push_back(s1);
				else{
					for(int k = 0; k < more_possibilities.size(); k++){
						string s2 = s1 + more_possibilities[k];
						possible_orders.push_back(s2);
					}
				}
				delete tempNewBoard;
			}
		}
		delete newBoard;
	}
	return possible_orders;
}

bool Board::check_terminal(){
	if(white_rings_out >= rings_to_remove || black_rings_out >= rings_to_remove)
		return true;
	return false;
}

Board* Board::copy_board(){
	Board *newBoard = new Board();
	newBoard->black_rings_in = this->black_rings_in;
	newBoard->black_rings_out = this->black_rings_out;
	newBoard->white_rings_in = this->white_rings_in;
	newBoard->white_rings_out = this->white_rings_out;
	newBoard->turn_id = this->turn_id;
	for(int i = 0; i < this->white_rings.size(); i++)
		newBoard->white_rings.push_back(this->white_rings[i]);
	for(int i = 0; i < this->black_rings.size(); i++)
		newBoard->black_rings.push_back(this->black_rings[i]);
	for(int i = 0; i < 2*board_size+1; i++){
		for(int j = 0; j < 2*board_size+1; j++){
			newBoard->board[i][j] = this->board[i][j];
		}
	}
	return newBoard;
}

void Board::printBoard(){
	for(int i = 0; i < 2*board_size+1; i++){
		for(int j = 0; j <2*board_size+1; j++){
			coordinates c;
			c.x = i;
			c.y = j;
			if (checkValid(c))
				cerr << board[i][j] << " ";
			else
				cerr << "  ";
			if(j == 2*board_size)
				cerr << endl;
		}
	}
}

void Board::iterate_over_line(int value, int c, int line_no, int start, int end){	// value = -1 for flip, 0 for remove //  c = 0 for x, 1 for y //
	for(int i = start; i <= end; i++){
		if(c == 0)
		{
			if(board[i][line_no] == 1)
				black_markers--;
			else if(board[i][line_no] == -1)
				white_markers--;
			board[i][line_no] *= value;
			if(board[i][line_no] == 1)
				black_markers++;
			else if(board[i][line_no] == -1)
				white_markers++;
		}
		else
		{
			if(board[line_no][i] == 1)
				black_markers--;
			else if(board[line_no][i] == -1)
				white_markers--;
			board[line_no][i] *= value;
			if(board[line_no][i] == 1)
				black_markers++;
			else if(board[line_no][i] == -1)
				white_markers++;		
		}
	}
}

vector<pair<coordinates, coordinates>> Board::get_markers_in_a_row(){
	vector<pair<coordinates,coordinates>> retVal;
	coordinates start_coord,end_coord;
	location start,end;
	for (int i=0;i<2*board_size+1;i++){
		for (int j=0;j<2*board_size+1;j++){
			// cerr<<"CHECKING COORDINATES "<<i<<" "<<j<<endl;
			start_coord.x=i;
			start_coord.y=j;
			if (!(checkValid(start_coord) && board[i][j]==turn_id))
				continue;
			
			//moving down
			// cerr<<"check down"<<endl;
			int counter=1,k=j+1,l;
			end_coord.x=i;
			end_coord.y=j-1;
			if (!checkValid(end_coord) || board[i][j-1]!=turn_id){
				end_coord.y=k;

				while(checkValid(end_coord))
				{
					if (board[i][k]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.y=++k;
					
				}
			}
			
			if (counter==markers_in_line)
				retVal.push_back(make_pair(start_coord,end_coord));

			// moving up
			// cerr<<"check up"<<endl;
			counter=1;
			k=j-1;
			end_coord.x=i;
			end_coord.y=j+1;
			if (!checkValid(end_coord) || board[i][j-1]!=turn_id){
				end_coord.y=k;

				while(checkValid(end_coord))
				{
					if (board[i][k]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.y=--k;
					
				}
			}
			if (counter==markers_in_line)
				retVal.push_back(make_pair(start_coord,end_coord));

			// moving right sideways
			// cerr<<"check right"<<endl;
			counter=1;
			k=i+1;
			end_coord.x=i-1;
			end_coord.y=j;
			if (!checkValid(end_coord) || board[i-1][j]!=turn_id){
				end_coord.x=k;

				while(checkValid(end_coord))
				{
					if (board[k][j]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.x=++k;
					
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving left sideways
			// cerr<<"check left"<<endl;
			counter=1;
			k=i-1;
			end_coord.x=i+1;
			end_coord.y=j;
			
			if (!checkValid(end_coord) || board[i+1][j]!=turn_id){
				end_coord.x=k;

				while(checkValid(end_coord))
				{
					if (board[k][j]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.x=--k;
					
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving diagonal right down
			// cerr<<"check diagonal right"<<endl;
			counter=1;
			k=i+1;
			l=j+1;
			end_coord.x=i-1;
			end_coord.y=j-1;
			if (!checkValid(end_coord) || board[i-1][j-1]!=turn_id){
				end_coord.x=k;
				end_coord.y=l;
				while(checkValid(end_coord))
				{
					if (board[k][l]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.x=++k;
					end_coord.y=++l;
					
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
			
			// moving diagonal right down
			// cerr<<"check diagonal leftt"<<endl;
			
			counter=1;
			k=i-1;
			l=j-1;
			end_coord.x=i+1;
			end_coord.y=j+1;
			if (!checkValid(end_coord) || board[i+1][j+1]!=turn_id){
				end_coord.x=k;
				end_coord.y=l;

				while(checkValid(end_coord))
				{
					if (board[k][l]!=turn_id)
						break;
					counter++;
					if (counter>=markers_in_line)
						break;
					end_coord.x=--k;
					end_coord.y=--l;
					
				}
			}
			if (counter==markers_in_line)			
				retVal.push_back(make_pair(start_coord,end_coord));
		}
	}
	return retVal;
}

std::pair<Board*, string> Board::moveRing_to_pair(Board* b, coordinates start, coordinates end, string removeMarker_string){
	string s = removeMarker_string;
	location inital_location = coordinates_to_location(start);
	s += "S " + to_string(inital_location.hexagon) + " " + to_string(inital_location.position) + " ";
	location newLocation = coordinates_to_location(end);
	s += "M " + to_string(newLocation.hexagon) + " " + to_string(newLocation.position) + " ";
	Board* newBoard = b->copy_board();
	newBoard->moveRing(start, end);

	newBoard->turn_id *= -1;

	//cout<<"*******************";
	//newBoard->printBoard();
	pair<Board*, string> p=make_pair(newBoard, s);
	return p;
}

string Board::moveRing_to_string(coordinates start, coordinates end, string removeMarker_string){
	string s = removeMarker_string;
	location inital_location = coordinates_to_location(start);
	s += "S " + to_string(inital_location.hexagon) + " " + to_string(inital_location.position) + " ";
	location newLocation = coordinates_to_location(end);
	s += "M " + to_string(newLocation.hexagon) + " " + to_string(newLocation.position) + " ";
	return s;
}

int Board::board_size;			// N
int Board::given_rings;		// M
int Board::rings_to_remove;	// L
int Board::markers_in_line;	// K

void Board::set_board_params(int board_size_in, int given_rings_in, int rings_to_remove_in, int markers_in_line_in){
	board_size = board_size_in;
	given_rings = given_rings_in;
	rings_to_remove = rings_to_remove_in;
	markers_in_line = markers_in_line_in;
}
