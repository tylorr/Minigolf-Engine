#include <cassert>

#include "file_handling.h"

hole readData(char* file)
{
	hole h;
	vector<string> line_data;  //vector to hold all elements in a single line of the input file
	ifstream fin;  //input stream for file-in.
	fin.open(file);

	while (fin) //while file has lines left
	{
		string s;  //string to store entire line

		if (!getline( fin, s )) break;  //if there are no more unread lines, break loop

		istringstream ss( s );  //creates input stream from string s
		while (ss)
		{
			string t;
			if (!getline( ss, t, ' ' )) break;  //get elements in line separated by whitespace
			line_data.push_back( t );  //push each element in the line onto vector
		}
		if(line_data.at(0) == "tile")
		{
			tile t;
			t.id = stringToInt(line_data.at(1));
			t.num_vertices = stringToInt(line_data.at(2));
			for(int i = 0; i < t.num_vertices; i++)  //go through all string elements that specify points
			{
				point p;
				p.x = stringToFloat(line_data.at(3+(i*3)));  //3,4, and 5 are the indices of the first set of point coordinates
				p.y = stringToFloat(line_data.at(4+(i*3)));
				p.z = stringToFloat(line_data.at(5+(i*3)));
				t.vertices.push_back(p);

				t.neighbors.push_back(stringToInt(line_data.at(3+(t.num_vertices*3)+i)));  //add neighbors as well
			}

			assert(t.vertices.size() == t.neighbors.size());

			h.tiles.push_back(t);  //add the tile to tile vector for the hole
		}
		else if(line_data.at(0) == "tee")  //assign values to tee object
		{
			point pos;

			h.tee.id = stringToInt(line_data.at(1));
			pos.x = stringToFloat(line_data.at(2));
			pos.y = stringToFloat(line_data.at(3));
			pos.z = stringToFloat(line_data.at(4));

			h.tee.position = pos;
		}
		else if(line_data.at(0) == "cup")  //assign values to cup object
		{
			point pos;

			h.cup.id = stringToInt(line_data.at(1));
			pos.x = stringToFloat(line_data.at(2));
			pos.y = stringToFloat(line_data.at(3));
			pos.z = stringToFloat(line_data.at(4));

			h.cup.position = pos;
		}
		else
		{
			cerr<<"Invalid object type";
		}
		line_data.clear();
	}

	if (!fin.eof())  //should be at the end of the file at this point
	{
		cerr << "Blah\n";
	}

	return h;
}

float stringToFloat(string s)  //util
{
	const char* p;
	p = s.c_str();
	return (float)atof(p);
}

int stringToInt(string s)  //util
{
	const char* p;
	p = s.c_str();
	return atoi(p);
}
