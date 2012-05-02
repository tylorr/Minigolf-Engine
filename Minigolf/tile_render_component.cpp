#include "Utils.h"

#include "tile_render_component.h"

using glm::vec3;
using glm::mat3;
using glm::mat4;
using glm::value_ptr;

using namespace std;

void TileRenderComponent::Receive(int message) {
}

void TileRenderComponent::Initialize(const tile &t) {
	LoadBuffers(t);
}

void TileRenderComponent::LoadBuffers(const tile &t) {
	Vertex *VERTICES = new Vertex[t.num_vertices];

	int N = t.num_vertices;

	this->vertexCount = N;

	int index = 0;
	int tIndex;
	point p;
	int count = 0;

	p = t.vertices[0];
	vec3 p0 = vec3(p.x, p.y, p.z);
	p = t.vertices[1];
	vec3 p1 = vec3(p.x, p.y, p.z);
	p = t.vertices[2];
	vec3 p2 = vec3(p.x, p.y, p.z);

	vec3 normal = glm::cross(p2 - p1, p0 - p1);
	normal = glm::normalize(normal);

	while (index < N)
	{
		switch (index % 2) {
		case 0:
			tIndex = (N - count) % N;
			count++;
			break;
		case 1:
			tIndex = count;
			break;
		}
		p = t.vertices[tIndex];

		VERTICES[index].Position[0] = p.x;
		VERTICES[index].Position[1] = p.y;
		VERTICES[index].Position[2] = p.z;

		VERTICES[index].Normal[0] = normal.x;
		VERTICES[index].Normal[1] = normal.y;
		VERTICES[index].Normal[2] = normal.z;

		index++;
	}

	const size_t BufferSize = N * sizeof(Vertex);
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

void TileRenderComponent::DestoryBuffers() {
	glDeleteBuffers(1, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void TileRenderComponent::Render(stack<mat4> *ModelViewMatrix, const GLuint &ModelViewMatrixUniformLocation, const GLuint &NormalMatrixUnifromLocation) {
	ModelViewMatrix->push(ModelViewMatrix->top());
	
	mat3 NormalMatrix = glm::inverse(glm::transpose(mat3(ModelViewMatrix->top())));

	glUniformMatrix3fv(NormalMatrixUnifromLocation, 1, GL_FALSE, value_ptr(NormalMatrix));
	glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, value_ptr(ModelViewMatrix->top()));
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
	ExitOnGLError("ERROR: Could not draw the cube");

	glBindVertexArray(0);

	ModelViewMatrix->pop();
}