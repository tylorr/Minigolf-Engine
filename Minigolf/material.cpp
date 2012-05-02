#include "material.h"

Material::Material(char *vertexShaderFile, char *fragmentShaderFile)
{
	this->vertexShaderFile = vertexShaderFile;
	this->fragmentShaderFile = fragmentShaderFile;
}

Material::~Material()
{

}

void Material::Initialize()
{
	this->shaderProgram = glCreateProgram();
	ExitOnGLError("ERROR: Could not create shader program");
	{
		this->vertexShader = LoadShader(this->vertexShaderFile, GL_VERTEX_SHADER);
		this->fragmentShader = LoadShader(this->fragmentShaderFile, GL_FRAGMENT_SHADER);
		glAttachShader(this->shaderProgram, this->vertexShader);
		glAttachShader(this->shaderProgram, this->fragmentShader);
	}
	glLinkProgram(this->shaderProgram);
	ExitOnGLError("ERROR: Could not link the shader program");

	this->modelViewMatrixUniformLocation = glGetUniformLocation(this->shaderProgram, "ModelViewMatrix");
	this->projectionMatrixUniformLocation = glGetUniformLocation(this->shaderProgram, "ProjectionMatrix");
	this->normalUniformMatrixLocation = glGetUniformLocation(this->shaderProgram, "NormalMatrix");
	ExitOnGLError("ERROR: Could not get shader uniform locations");
	double p = PI;
}

const GLint Material::GetModelViewUniformLocation() const
{
	return modelViewMatrixUniformLocation;
}

const GLint Material::GetProjectUniformLocation() const
{
	return projectionMatrixUniformLocation;
}