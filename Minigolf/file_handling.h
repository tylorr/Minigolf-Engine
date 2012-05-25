#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>

#include "glm\glm.hpp"

using std::vector;
using std::string;
using glm::vec3;

//==========================STRUCTS================================//

struct Tile {  //holds info about each tile
	int id;
	int num_vertices;
	vector<vec3> vertices;
	vector<int> neighbors;
};

struct TeeCup {  //holds info about a tee or a cup(same info needed)
	int id;
	vec3 position;
};

struct Hole {  //holds info about each hole
	vector<Tile> tiles;
	TeeCup tee;
	TeeCup cup;
	int par;
	string name;

	void clear(){tiles.clear();}
};

//==========================FUNCTIONS================================//
float stringToFloat(string s);  //converts a string to float
int stringToInt(string s);  //converts a string to int
vector<Hole> readData(char* file);

#endif // FILE_HANDLING_H
