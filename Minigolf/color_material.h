#ifndef COLOR_MATERIAL_H
#define COLOR_MATERIAL_H

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "glm\glm.hpp"

#include "material.h"

using glm::vec3;

/*
	remarks:	This does not currently work
*/
class ColorMaterial : public Material {
public:

	vec3 color_;

	ColorMaterial(const std::string &shader_program_key, const vec3 &color);

	virtual void Initialize();
	virtual void PreRender();

private:
	GLint color_uniform_;
};

#endif // COLOR_MATERIAL_H