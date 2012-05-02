#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utils.h"

class Material {
public:
	Material(char *vertexShaderFile, char *fragmentShaderFile);
	virtual ~Material();

	virtual void Initialize();
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;

	const GLint GetModelViewUniformLocation() const;
	const GLint GetProjectUniformLocation() const;

private:
	char *vertexShaderFile;
	char *fragmentShaderFile;

	GLuint shaderProgram; 
	GLuint vertexShader;
	GLuint fragmentShader;

	GLint modelViewMatrixUniformLocation;
	GLint projectionMatrixUniformLocation;
	GLint normalUniformMatrixLocation;
};

#endif // MATERIAL_H