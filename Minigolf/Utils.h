#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include "glm\glm.hpp"

void ExitOnGLError(const char* error_message);

/*
	input:		point to be projected, normal and point representing plane

	output:		the projections of point onto plane
*/
glm::vec3 Project(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &point_on_plane);


/*
	input:		point, and 2D vertices defining polygon

	output:		does the polygon contain the point?

	remarks:	If using a 3d, try projecting the vertices
				to a plane by removing one of the elements.
*/
bool PointInPolygon(glm::vec2 point, std::vector<glm::vec2> vertices);

#endif
