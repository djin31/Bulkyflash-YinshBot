#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;
extern int board_size;

struct location
{
	int hexagon;
	int position;

};

struct coordinates
{
	int x;	//vertical line
	int y;	//line 60* to the horizontal (CCW)
};

template<typename Out>
void split(const std::string &s, char delim, Out result);
std::vector<std::string> split(const std::string &s, char delim);
coordinates location_to_coordinates(location l);
location coordinates_to_location(coordinates c);
bool checkValid(coordinates c);

#endif