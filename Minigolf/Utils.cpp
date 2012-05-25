#include "GL\glew.h"
#include "GL\freeglut.h"

#include "Utils.h"

void ExitOnGLError(const char* error_message)
{
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
