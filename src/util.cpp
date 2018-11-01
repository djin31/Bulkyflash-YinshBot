#include "util.h"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

location coordinates_to_location(coordinates c){
	c.x-=board_size;
	c.y=board_size-c.y;

	location l;
	if (c.x>=0 && c.y>=0)
	{
		l.hexagon=abs(c.x+c.y);
		l.position=c.x;
	}
	else if (c.x>=0 && c.y<0)
	{
		l.hexagon=max(abs(c.x),abs(c.y));
		if (c.x==l.hexagon)
			l.position=l.hexagon-c.y;
		else
			l.position=3*l.hexagon-c.x;
	}
	else if (c.x<0 && c.y<0)
	{
		l.hexagon=abs(c.x+c.y);		
		l.position=3*l.hexagon-c.x;		
	}
	else
	{
		l.hexagon=max(abs(c.x),abs(c.y));
		if (c.x== -(l.hexagon))
			l.position=4*l.hexagon+c.y;
		else
			l.position=(6*l.hexagon+c.x)%(6*l.hexagon);
	}
	return l;
}

coordinates location_to_coordinates(location l){
	coordinates c;
	c.x = board_size;
	c.y = board_size;
	int h = l.hexagon, p = l.position;
	if (h == 0){
		return c;	
	}
	switch(p/h){
		case 0 : c.x += p; c.y += p-h; break;
		case 1 : c.x += h; c.y += p-h; break;
		case 2 : c.x += 3*h-p; c.y += h; break; 
		case 3 : c.x += 3*h-p; c.y += 4*h-p; break;
		case 4 : c.x += -1*h; c.y += 4*h-p; break;
		case 5 : c.x += p-6*h; c.y += -1*h; break;
		default : break;
	}
	return c;
}

bool checkValid(coordinates c){
	location l = coordinates_to_location(c);
	if(l.hexagon == 0 && l.position == 0)
		return true;
	if (l.hexagon==board_size)
		return (l.position<6*l.hexagon && l.position%l.hexagon);
	else
		return (l.hexagon<board_size && l.position<6*l.hexagon);
	return false;
}