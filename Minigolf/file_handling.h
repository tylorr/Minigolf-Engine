#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>

using std::vector;
using std::string;

//==========================STRUCTS================================//
struct Point {  //stores a point in 3d space
	float x;
	float y;
	float z;
};

struct Tile {  //holds info about each tile
	int id;
	int num_vertices;
	vector<Point> vertices;
	vector<int> neighbors;
};

struct TeeCup {  //holds info about a tee or a cup(same info needed)
	int id;
	Point position;
};

struct Hole {  //holds info about each hole
	vector<Tile> tiles;
	TeeCup tee;
	TeeCup cup;
};

//==========================FUNCTIONS================================//
float stringToFloat(string s);  //converts a string to float
int stringToInt(string s);  //converts a string to int
Hole readData(char* file);

#endif // FILE_HANDLING_H
