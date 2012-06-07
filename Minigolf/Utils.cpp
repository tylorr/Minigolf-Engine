#include <cfloat>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "Utils.h"

void ExitOnGLError(const char* error_message) {
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		const char* APPEND_DETAIL_STRING = ": %s\n";
		const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
		const size_t message_length = strlen(error_message);
		char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

		memcpy(display_message, error_message, message_length);
		memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

		fprintf(stderr, display_message, gluErrorString(ErrorValue));

		free(display_message);
		exit(EXIT_FAILURE);
	}
}

glm::vec3 Project(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &point_on_plane) {
	using glm::vec3;

	// calculate D from Ax + Bx + Cx + D = 0 or N Dot POP = -D
	float D = -glm::dot(normal, point_on_plane);

	vec3 proj = point - ((glm::dot(normal, point) + D) * normal);
	return proj;
}

bool PointInPolygon(glm::vec2 point, std::vector<glm::vec2> vertices) {
	using glm::vec2;
	
	float x1, x2;
	int i, size, crossings = 0;
	vec2 v1, v2;

	// iterate through each edge of polygon
	for (i = 0, size = vertices.size(); i < size; ++i) {
		v1 = vertices[i];
		v2 = vertices[(i + 1) % size];

		// ensure that we get same value from right to left and
		// from left to right
		if (v1.x < v2.x) {
			x1 = v1.x;
			x2 = v2.x;
		} else {
			x1 = v2.x;
			x2 = v1.x;
		}

		// is it possible for the ray to cross the line?
		if (point.x > x1 && point.x <= x2 && (point.y < v1.y || point.y <= v2.y)) {
			static const float eps = 0.000001f;

			// calculate equation of line
			vec2 d = v2 - v1;
			float k;

			if (glm::abs(d.x) < eps) {
				k = FLT_MAX;
			} else {
				k = d.y / d.x;
			}

			float m = v1.y - k * v1.x;

			// Find if the ray crosses the line
			float y2 = k * point.x + m;
			if (point.y < y2) {
				crossings++;
			}
		}
	}

	// is there an odd number of crossings?
	return (crossings % 2) == 1;
}
