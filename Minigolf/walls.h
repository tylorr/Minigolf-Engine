#ifndef WALLS_H
#define WALLS_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"

struct Walls : public Component {
	std::vector<boost::shared_ptr<Entity>> walls;

	Walls() : Component("Walls") { }
}; // struct Walls

#endif // WALLS_H