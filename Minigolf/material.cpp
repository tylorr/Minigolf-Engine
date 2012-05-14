#include "material.h"

#include "shader_cache.h"
#include "Utils.h"

Material::Material(const std::string &shader_program_key){
	shader_program_ = ShaderCache::GetShaderProgram(shader_program_key);
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
	ExitOnGLError("ERROR: Cannot use program");
}

void Material::PostRender() {
	glUseProgram(0);
}

void Material::PushMatrices(const mat4 &model_view, const mat4 &projection, const mat4 &mvp, const mat4 &normal) {
	using glm::value_ptr;

	glUniformMatrix4fv(model_view_uniform_, 1, GL_FALSE, value_ptr(model_view));
	ExitOnGLError("ERROR: Cannot use push model_view");

	glUniformMatrix4fv(projection_uniform_, 1, GL_FALSE, value_ptr(projection));
	ExitOnGLError("ERROR: Cannot use push projection");

	glUniformMatrix4fv(mvp_uniform_, 1, GL_FALSE, value_ptr(mvp));
	ExitOnGLError("ERROR: Cannot use push mvp");

	glUniformMatrix3fv(normal_uniform_, 1, GL_FALSE, value_ptr(normal));
	ExitOnGLError("ERROR: Cannot use push normal");
	
}