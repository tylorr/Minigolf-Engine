#ifndef UTILS_H
#define UTILS_H

#include "glm\glm.hpp"

void ExitOnGLError(const char* error_message);

glm::vec3 Project(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &point_on_plane);

#endif
