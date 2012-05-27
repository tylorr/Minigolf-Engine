#ifndef TILE_COMPONENT_H
#define TILE_COMPONENT_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"

struct TileComponent : public Component {
	std::vector<EntityPtr> neighbors;
	std::vector<EntityPtr> walls;

	bool has_cup;
	EntityPtr cup;

	TileComponent() : Component("TileComponent") { }
};

typedef boost::shared_ptr<TileComponent> TileComponentPtr;

#endif // TILE_COMPONENT_H