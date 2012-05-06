#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "glm\glm.hpp"

#include "material.h"

using glm::vec4;
using glm::vec3;

class BasicMaterial : public Material {
public:

	// TODO: move light information into its own component
	vec4 light_position_;
	vec3 Ld_;
	vec3 Kd_;

	BasicMaterial(const std::string &shader_program_key, const vec4 &light_position, const vec3 &Ld, const vec3 &Kd);

	virtual void Initialize();
	virtual void PreRender();
	
private:
	GLint light_position_uniform_;
	GLint Kd_uniform_;
	GLint Ld_uniform_;
};


#endif // BASIC_MATERIAL_H