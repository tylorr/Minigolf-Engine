#ifndef TEE_RENDER_COMPOENT_H
#define TEE_RENDER_COMPOENT_H

#include <stack>

#include "component.h"
#include "Utils.h"
#include "filehandling.h"

class TeeRenderComponent : public Component {
public:

	void Initialize(const teecup &t);
	void LoadBuffers(const teecup &t);
	void DestoryBuffers();
	void Render(std::stack<glm::mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation);

	virtual void Receive(int message);

private:
	GLuint BufferIds[3];
	unsigned vertexCount;
};

#endif