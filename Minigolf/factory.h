#ifndef FACTORY_H
#define FACTORY_H

#include <boost\shared_ptr.hpp>

#include "file_handling.h"

class Entity;
class Material;
struct Transform;

namespace Factory {

boost::shared_ptr<Entity> CreateCamera(const float &fov, const float &aspect, const float &near_plane, const float &far_plane);

void CreateLevel(const Hole &hole);
boost::shared_ptr<Entity> CreateTile(const Tile &tile, const boost::shared_ptr<Material> &material);
boost::shared_ptr<Entity> CreateBall(const TeeCup &tee);
boost::shared_ptr<Entity> CreateTee(const TeeCup &tee);
boost::shared_ptr<Entity> CreateCup(const TeeCup &cup);

}; // namespace Factory

#endif // FACTORY_H