#include "shader_cache.h"

#include "material.h"

Material::Material(std::string shaderProgramKey){
	shader_program_ = shader_cache::GetShaderProgram(shaderProgramKey);
}

void Material::Initialize() {
	model_view_uniform_ = glGetUniformLocation(shader_program_, "ModelViewMatrix");
	projection_uniform_ = glGetUniformLocation(shader_program_, "ProjectionMatrix");
	mvp_uniform_ = glGetUniformLocation(shader_program_, "MVP");
	normal_uniform_ = glGetUniformLocation(shader_program_, "NormalMatrix");
	
	ExitOnGLError("ERROR: Could not get shader uniform locations");
}

void Material::PushMatrices(const mat4 &model_view, const mat4 &projection, const mat4 &mvp, const mat4 &normal) {
	using glm::value_ptr;

	glUniformMatrix3fv(model_view_uniform_, 1, GL_FALSE, value_ptr(model_view));
	glUniformMatrix3fv(projection_uniform_, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix3fv(mvp_uniform_, 1, GL_FALSE, value_ptr(mvp));
	glUniformMatrix3fv(normal_uniform_, 1, GL_FALSE, value_ptr(normal));
}