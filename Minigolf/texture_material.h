#ifndef TEXTURE_MATERIAL_H
#define TEXTURE_MATERIAL_H

#include "material.h"

class TextureMaterial : public Material {
public:
	TextureMaterial(const std::string &shader_program_key, const std::string &texture_key);

	virtual void Initialize();
	virtual void PreRender();
	
private:
	GLint texture_uniform_;
	GLuint texture_;
};

#endif // TEXTURE_MATERIAL_H