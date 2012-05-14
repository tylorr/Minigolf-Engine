#include "basic_material.h"

#include "Utils.h"

BasicMaterial::BasicMaterial(const std::string &shader_program_key, const vec4 &light_position, const vec3 &Ld, const vec3 &Kd)
	: Material(shader_program_key) 
{
	light_position_ = light_position;
	Ld_ = Ld;
	Kd_ = Kd;
}

void BasicMaterial::Initialize() {
	Material::Initialize();

	light_position_uniform_ = glGetUniformLocation(shader_program_, "LightPosition");
	Kd_uniform_ = glGetUniformLocation(shader_program_, "Kd");
	Ld_uniform_ = glGetUniformLocation(shader_program_, "Ld");

	ExitOnGLError("ERROR: Could not get shader uniform locations");
}

void BasicMaterial::PreRender() {
	using glm::value_ptr;

	// make this is called to attach to shader program
	Material::PreRender();

	glUniform4fv(light_position_uniform_, 1, value_ptr(light_position_));
	glUniform3fv(Kd_uniform_, 1, value_ptr(Kd_));
	glUniform3fv(Ld_uniform_, 1, value_ptr(Ld_));
	ExitOnGLError("ERROR: Upload light");
}