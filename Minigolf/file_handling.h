#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//==========================STRUCTS================================//
struct point {  //stores a point in 3d space
	float x;
	float y;
	float z;
};

typedef struct{  //holds info about each tile
	int id;
	int num_vertices;
	vector<point> vertices;
	vector<int> neighbors;
}tile;

typedef struct{  //holds info about a tee or a cup(same info needed)
	int id;
	point position;
}teecup;

typedef struct{  //holds info about each hole
	vector<tile> tiles;
	teecup tee;
	teecup cup;
}hole;

//==========================FUNCTIONS================================//
float stringToFloat(string s);  //converts a string to float
int stringToInt(string s);  //converts a string to int
hole readData(char* file);

#endif // FILE_HANDLING_H
