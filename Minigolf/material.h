#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "Utils.h"

namespace render {

class Material {
public:
	Material(std::string shaderProgramKey);
	virtual ~Material() { }

	virtual void Initialize();
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;

	const GLint model_view_uniform() const;
	const GLint projection_uniform() const;

private:
	GLuint shader_program_;

	GLint model_view_uniform_;
	GLint projection_uniform_;
	GLint normal_uniform_;
};

}; // namespace render

#endif // MATERIAL_H