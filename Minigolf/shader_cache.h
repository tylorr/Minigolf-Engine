#ifndef SHADER_CAHCE_H
#define SHADER_CAHCE_H

#include <unordered_map>
#include <string>

#include "GL\glew.h"
#include "GL\freeglut.h"

/*
 * Use these methods to load shaders into a cache of shader programs
 */

namespace shader_cache {

// stores information about the shader program and shaders
struct ShaderInfo {
	GLuint program_;
	GLuint vertex_;
	GLuint fragment_;

	ShaderInfo() {
		program_ = 0;
		vertex_ = 0;
		fragment_ = 0;
	}

	ShaderInfo(const GLuint &program, const GLuint &vertex, const GLuint &fragment) 
		: program_(program),
		  vertex_(vertex),
		  fragment_(fragment) { }
};

typedef std::unordered_map<std::string, ShaderInfo> ShaderMap;

static ShaderMap *shader_map;

void Initialize();
void Destroy();

GLuint LoadShader(const char* filename, const GLenum &shader_type);

void AddShader(const std::string &key, const char *vertex_file, const char *fragment_file);
const GLuint GetShaderProgram(std::string key);

}; // namespace render

#endif // SHADER_CAHCE_H