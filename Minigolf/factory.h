#ifndef FACTORY_H
#define FACTORY_H

#include <boost\shared_ptr.hpp>

#include "file_handling.h"

class Entity;
class Material;

namespace Factory {

boost::shared_ptr<Entity> CreateCamera(const float &fov, const float &aspect, const float &near_plane, const float &far_plane);

void CreateLevel(const Hole &hole);
void CreateTile(const Tile &tile, boost::shared_ptr<Material>);

}; // namespace Factory

#endif // FACTORY_H