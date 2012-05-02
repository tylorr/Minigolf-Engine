#ifndef SHADER_CAHCE_H
#define SHADER_CAHCE_H

#include <unordered_map>
#include <string>

#include "Utils.h"

/*
 * Use these methods to load shaders into a cache of shader programs
 */

namespace render {

struct ShaderInfo {
	GLuint program;
	GLuint vertex;
	GLuint fragment;

	ShaderInfo() {
		program = 0;
		vertex = 0;
		fragment = 0;
	}

	ShaderInfo(const GLuint &program, const GLuint &vertex, const GLuint &fragment) 
		: program(program),
		  vertex(vertex),
		  fragment(fragment) { }
};

typedef std::unordered_map<std::string, ShaderInfo> ShaderMap;

static ShaderMap shader_map;

GLuint LoadShader(const char* filename, GLenum shader_type);
void AddShader(const std::string &key, const char *vertexFile, const char *fragmentFile);
const GLuint GetShaderProgram(std::string key);

}; // namespace render

#endif // SHADER_CAHCE_H