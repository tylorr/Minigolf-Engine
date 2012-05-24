#ifndef TILE_COMPONENT_H
#define TILE_COMPONENT_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"

struct TileComponent : public Component {
	std::vector<boost::shared_ptr<Entity>> neighbors;
	std::vector<boost::shared_ptr<Entity>> walls;

	bool has_cup;
	boost::shared_ptr<Entity> cup;

	TileComponent() : Component("TileComponent") { }
};

#endif // TILE_COMPONENT_H