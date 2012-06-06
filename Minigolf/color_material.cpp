#include "glm\gtc\type_ptr.hpp"

#include "color_material.h"
#include "Utils.h"

ColorMaterial::ColorMaterial(const std::string &shader_program_key, const vec3 &color) 
	: Material(shader_program_key) {
		color_ = color;
}

void ColorMaterial::Initialize() {
	Material::Initialize();

	color_uniform_ = glGetUniformLocation(shader_program_, "Color");
}

void ColorMaterial::PreRender() {
	using glm::value_ptr;

	Material::PreRender();

	glUniform3fv(color_uniform_, 1, value_ptr(color_));
	ExitOnGLError("ERROR: Could not upload color uniform");
}