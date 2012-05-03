#ifndef TILE_RENDER_COMPONENT_H
#define TILE_RENDER_COMPONENT_H

#include <stack>

#include "component.h"
#include "Utils.h"
#include "file_handling.h"

class TileRenderComponent : public Component {
public:

	void Initialize(const tile &t);
	void LoadBuffers(const tile &t);
	void DestoryBuffers();
	void Render(std::stack<glm::mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &NormalMatrixUnifromLocation);

	virtual void Receive(int message);

private:
	GLuint BufferIds[3];
	unsigned vertexCount;
};

#endif