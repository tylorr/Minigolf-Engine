#include "Utils.h"

#include "tee_render_component.h"

using namespace std;
using namespace glm;

void TeeRenderComponent::Receive(int message) {
}

void TeeRenderComponent::Initialize(const teecup &t) {
	LoadBuffers(t);
}

void TeeRenderComponent::LoadBuffers(const teecup &t) {
	Vertex VERTICES[3] = {
	};

	const size_t BufferSize = 3 * sizeof(Vertex);
	const size_t VertexSize = sizeof(VERTICES[0]);
	const size_t NormalOffset = sizeof(VERTICES[0].Position);


	glGenVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	glGenBuffers(1, &BufferIds[1]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, VERTICES, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)NormalOffset);
	ExitOnGLError("ERROR: Could not set VAO attributes");

	glBindVertexArray(0);
}

void TeeRenderComponent::DestoryBuffers() {
	glDeleteBuffers(1, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void TeeRenderComponent::Render(stack<mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation) {
	ModelViewMatrix->push(ModelViewMatrix->top());

	glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, value_ptr(ModelViewMatrix->top()));
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
	ExitOnGLError("ERROR: Could not draw the cube");

	glBindVertexArray(0);

	ModelViewMatrix->pop();
}