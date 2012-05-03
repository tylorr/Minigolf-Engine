#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "file_handling.h"

class Entity;

class Level {
public:
	vector<Entity *> tiles;

	static Level *CreateLevel(const hole &h);
	
};

#endif // LEVEL_H