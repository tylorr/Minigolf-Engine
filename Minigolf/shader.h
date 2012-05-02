#ifndef SHADER_H
#define SHADER_H

#include "Utils.h"

namespace render {

static GLuint diffuseShader;

void Initialize();
const GLuint GetDiffuseShader();


} // namespace render

#endif // SHADER_H