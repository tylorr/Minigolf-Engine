#include <map>

#include "SOIL.h"

#include "texture_cache.h"

using std::string;
using std::map;

namespace TextureCache {

namespace {
	typedef map<string, GLuint> TextureMap;

	TextureMap texture_map;

	GLuint Create(const char *filename) {
		GLuint tex_2d = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		if(!tex_2d)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
			exit(EXIT_FAILURE);
		}

		return tex_2d;
	}
}; // namespace

GLuint FindOrCreate(const string &filename) {
	GLuint texture = Find(filename);

	if (!texture) {
		texture = Create(filename.c_str());
		texture_map[filename] = texture;
	}

	return texture;
}

GLuint Find(const std::string &filename) {
	return texture_map[filename];
}

}; // namespace TextureCache