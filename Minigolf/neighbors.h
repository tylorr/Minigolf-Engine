#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"

struct Neighbors : public Component {
	std::vector<boost::shared_ptr<Entity>> neighbors;

	Neighbors() : Component("Neighbors") { }

};

#endif // NEIGHBORS_H