#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "glm\glm.hpp"

using glm::mat4;

class Material {
public:
	Material(std::string shaderProgramKey);
	virtual ~Material() { }

	virtual void Initialize();
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;

	void PushMatrices(const mat4 &model_view, const mat4 &projection, const mat4 &mvp, const mat4 &normal);

private:
	GLuint shader_program_;

	GLint model_view_uniform_;
	GLint projection_uniform_;
	GLint mvp_uniform_;
	GLint normal_uniform_;
};

#endif // MATERIAL_H