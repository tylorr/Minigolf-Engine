#include "shader_cache.h"

#include "material.h"

namespace render {

Material::Material(std::string shaderProgramKey){
	shader_program_ = render::GetShaderProgram(shaderProgramKey);
}

void Material::Initialize() {
	model_view_uniform_ = glGetUniformLocation(shader_program_, "ModelViewMatrix");
	projection_uniform_ = glGetUniformLocation(shader_program_, "ProjectionMatrix");
	normal_uniform_ = glGetUniformLocation(shader_program_, "NormalMatrix");
	ExitOnGLError("ERROR: Could not get shader uniform locations");
}


const GLint Material::model_view_uniform() const {
	return model_view_uniform_;
}

const GLint Material::projection_uniform() const {
	return projection_uniform_;
}

}; // namespace render