#include "level.h"
#include "entity.h"
#include "component.h"
#include "tile_render_component.h"
#include "tile_render_manager.h"

Level *Level::CreateLevel(const Hole &h) {
	Level *level = new Level();
	vector<Tile>::const_iterator it;

	Entity *tile;
	for (it = h.tiles.begin(); it != h.tiles.end(); it++) {
		tile = TileRenderManager::CreateTile(*it);
		level->tiles.push_back(tile);
	}

	return level;
};