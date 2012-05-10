#include "mesh.h"

// TODO: Maybe this should go in the constructor
void Mesh::Initialize(const VertexType &vertex_type, const Vertex *vertices, const GLsizei &vertex_count, const GLuint *indices, const GLsizei &index_count) {
	const size_t vbo_size = vertex_count * sizeof(Vertex);
	const size_t ibo_size = index_count * sizeof(unsigned int);
	const size_t vertex_size = sizeof(vertices[0]);
	const size_t normal_offset = sizeof(vertices[0].Position);
	const size_t texture_offset = normal_offset + sizeof(vertices[0].Normal);

	this->vertex_type_ = vertex_type;
	this->index_count_ = index_count;

	glGenVertexArrays(1, &vertex_array_object_);
	ExitOnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(vertex_array_object_);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glGenBuffers(1, &vertex_buffer_object_);
	ExitOnGLError("ERROR: Could not generate the VBO");

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, vbo_size, vertices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");
	
	switch(vertex_type_) {
	case POSITION_NORMAL_TEX:
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)texture_offset);
	case POSITION_NORMAL:
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)normal_offset);
	case POSITION:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)0);
		break;
	}
	ExitOnGLError("ERROR: Could not enable or set VAO attributes");

	glGenBuffers(1, &index_buffer_object_);
	ExitOnGLError("ERROR: Could not generate the IBO");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, indices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);
}

void Mesh::Destroy() {
	glDeleteBuffers(1, &vertex_buffer_object_);
	glDeleteBuffers(1, &index_buffer_object_);
	glDeleteVertexArrays(1, &vertex_array_object_);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void Mesh::Draw() const {
	glBindVertexArray(vertex_array_object_);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	// note: hard-coded use of GL_TRIANGLE_STRIP, could be made user-defined
	glDrawElements(GL_TRIANGLE_STRIP, index_count_, GL_UNSIGNED_INT, (GLvoid*)0);
	ExitOnGLError("ERROR: Could not draw the mesh");

	glBindVertexArray(0);
};