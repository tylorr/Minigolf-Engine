#ifndef TILE_RENDER_COMPONENT_H
#define TILE_RENDER_COMPONENT_H

#include <stack>

#include "component.h"
#include "Utils.h"
#include "file_handling.h"

using glm::mat4;

class TileRenderComponent : public Component {
public:

	void Initialize(const Tile &t);
	void LoadBuffers(const Tile &t);
	void DestoryBuffers();
	void Render(std::stack<mat4> *ModelViewMatrix, const mat4 &ProjectionMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &ProjectionMatrixUniform, const GLuint &NormalMatrixUnifromLocation);

	virtual void Receive(int message);

private:
	GLuint BufferIds[3];
	unsigned vertexCount;
};

#endif