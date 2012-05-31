#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <string>

#include "GL\glew.h"
#include "GL\freeglut.h"

namespace TextureCache {
	GLuint FindOrCreate(const std::string &filename);
	GLuint Find(const std::string &filename);
};

#endif // TEXTURE_CACHE_H