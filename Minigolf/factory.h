#ifndef FACTORY_H
#define FACTORY_H

#include <boost\shared_ptr.hpp>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "file_handling.h"
#include "entity.h"

class Material;
class Geometry;
struct Transform;

namespace Factory {

EntityPtr CreateCamera(const float &fov, const float &aspect, const float &near_plane, const float &far_plane);

void CreateLevel(const Hole &hole);
EntityPtr CreateTile(const Tile &tile, const boost::shared_ptr<Material> &material);
EntityPtr CreateWall(const vec3 &tile_normal, const vec3 &p1, const vec3 &p2);
EntityPtr CreateBall(const TeeCup &tee);
EntityPtr CreateTee(const TeeCup &tee);
EntityPtr CreateCup(const TeeCup &cup);

boost::shared_ptr<Geometry> Planar(const GLuint &program, const vector<vec3> &vertex_list);

}; // namespace Factory

#endif // FACTORY_H