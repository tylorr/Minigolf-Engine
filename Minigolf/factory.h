#ifndef FACTORY_H
#define FACTORY_H

#include <vector>

#include "glm\glm.hpp"

class Entity;
class Material;
class Camera;

namespace Factory {

Entity *CreateTile(Camera *camera, Material *material, const std::vector<glm::vec3> &vertex_list);

}; // namespace Factory

#endif // FACTORY_H