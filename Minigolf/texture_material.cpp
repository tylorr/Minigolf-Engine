#include "glm\gtc\type_ptr.hpp"

#include "texture_material.h"
#include "texture_cache.h"
#include "Utils.h"

TextureMaterial::TextureMaterial(const std::string &shader_program_key, const std::string &texture_key) 
	: Material(shader_program_key) {
		texture_ = TextureCache::FindOrCreate(texture_key);
}

void TextureMaterial::Initialize() {
	Material::Initialize();

	texture_uniform_ = glGetUniformLocation(shader_program_, "DiffuseTexture");

	ExitOnGLError("ERROR: Could not get shader uniform locations");
}

void TextureMaterial::PreRender() {
	using glm::value_ptr;

	// make this is called to attach to shader program
	Material::PreRender();

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glUniform1i(texture_uniform_, 0);
	ExitOnGLError("ERROR: Upload light");
}