#include "shader_cache.h"

namespace render {

void InitializeShaderCache() {
	shader_map = new ShaderMap();
}

void AddShader(const std::string &key, const char *vertex_file, const char *fragment_file) {
	GLuint program, vertex, fragment;
	ShaderMap::iterator it;

	it = shader_map->find(key);
	if (it != shader_map->end()) {
		fprintf(stderr, "ERROR: shader_cache: the key %s already exists\n", key);
		return;
	}

	program = glCreateProgram();
	ExitOnGLError("ERROR: Could not create the shader program");
	{
		fragment = LoadShader(fragment_file, GL_FRAGMENT_SHADER);
		vertex = LoadShader(vertex_file, GL_VERTEX_SHADER);
		glAttachShader(program, fragment);
		glAttachShader(program, vertex);
	}
	glLinkProgram(program);

	ShaderInfo si(program, vertex, fragment);

	(*shader_map)[key] = si;
}
const GLuint GetShaderProgram(std::string key) {
	ShaderInfo si = (*shader_map)[key];
	return si.program_;
}

GLuint LoadShader(const char* filename, const GLenum &shader_type) {
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	char* glsl_source;

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);

		if (NULL != (glsl_source = (char*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file))
			{
				glsl_source[file_size] = '\0';

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, (const GLchar **)&glsl_source, NULL);
					glCompileShader(shader_id);
					ExitOnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);

		fclose(file);
	}
	else
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);

	return shader_id;
}

void DestroyShaderCache() {
	GLuint program, vertex, fragment;

	for (ShaderMap::iterator it = shader_map->begin(); it != shader_map->end(); ++it) {
		program = it->second.program_;
		vertex = it->second.vertex_;
		fragment = it->second.fragment_;

		glDetachShader(program, vertex);
		glDetachShader(program, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(program);
		ExitOnGLError("ERROR: Could not destroy the shaders");
	}

	delete shader_map;
}

}; // namespace render