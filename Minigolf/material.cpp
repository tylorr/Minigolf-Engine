#include "material.h"

#include "shader_cache.h"
#include "Utils.h"

Material::Material(const std::string &shader_program_key){
	shader_program_ = shader_cache::GetShaderProgram(shader_program_key);
}

void Material::Initialize() {
	model_view_uniform_ = glGetUniformLocation(shader_program_, "ModelViewMatrix");
	projection_uniform_ = glGetUniformLocation(shader_program_, "ProjectionMatrix");
	mvp_uniform_ = glGetUniformLocation(shader_program_, "MVP");
	normal_uniform_ = glGetUniformLocation(shader_program_, "NormalMatrix");
	
	ExitOnGLError("ERROR: Could not get shader uniform locations");
}

void Material::PreRender() {
	glUseProgram(shader_program_);
}

void Material::PostRender() {
	glUseProgram(0);
}

void Material::PushMatrices(const mat4 &model_view, const mat4 &projection, const mat4 &mvp, const mat4 &normal) {
	using glm::value_ptr;

	glUniformMatrix3fv(model_view_uniform_, 1, GL_FALSE, value_ptr(model_view));
	glUniformMatrix3fv(projection_uniform_, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix3fv(mvp_uniform_, 1, GL_FALSE, value_ptr(mvp));
	glUniformMatrix3fv(normal_uniform_, 1, GL_FALSE, value_ptr(normal));
}