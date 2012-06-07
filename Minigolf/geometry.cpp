#include "geometry.h"

Geometry::~Geometry() {
}

// TODO: Maybe this should go in the constructor
void Geometry::Initialize(const GLuint &program, const GLenum &draw_mode, const VertexType &vertex_type, const Vertex *vertices, const GLsizei &vertex_count, const GLuint *indices, const GLsizei &index_count) {
	draw_mode_ = draw_mode;

	const size_t vbo_size = vertex_count * sizeof(Vertex);
	const size_t ibo_size = index_count * sizeof(unsigned int);

	vertex_size_ = sizeof(vertices[0]);
	normal_offset_ = sizeof(vertices[0].Position);
	texture_offset_ = normal_offset_ + sizeof(vertices[0].Normal);

	this->vertex_type_ = vertex_type;
	this->index_count_ = index_count;

	//glGenVertexArrays(1, &vertex_array_object_);
	//ExitOnGLError("ERROR: Could not generate the VAO");
	//glBindVertexArray(vertex_array_object_);
	//ExitOnGLError("ERROR: Could not bind the VAO");

	glGenBuffers(1, &vertex_buffer_object_);
	ExitOnGLError("ERROR: Could not generate the VBO");

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, vbo_size, vertices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");
	
	
	switch(vertex_type_) {
	case POSITION_NORMAL_TEX:
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)texture_offset);
		tex_index_ = glGetAttribLocation(program, "TexCoord");
		ExitOnGLError("ERROR: Could not get attribute tex");
	case POSITION_NORMAL:
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)normal_offset);
		normal_index_ = glGetAttribLocation(program, "VertexNormal");
		ExitOnGLError("ERROR: Could not get attribute normal");
	case POSITION:
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)0);
		position_index_ = glGetAttribLocation(program, "VertexPosition");
		ExitOnGLError("ERROR: Could not get attribute position");
		break;
	}
	ExitOnGLError("ERROR: Could not enable or set VAO attributes");
	

	glGenBuffers(1, &index_buffer_object_);
	ExitOnGLError("ERROR: Could not generate the IBO");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, indices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	//glBindVertexArray(0);
}

void Geometry::Destroy() {
	glDeleteBuffers(1, &vertex_buffer_object_);
	glDeleteBuffers(1, &index_buffer_object_);
	//glDeleteVertexArrays(1, &vertex_array_object_);

	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void Geometry::Draw() const {
	//glBindVertexArray(vertex_array_object_);
	//ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	// note: hard-coded use of GL_TRIANGLE_STRIP, could be made user-defined
	// todo: make drawing mode user-defined

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	switch(vertex_type_) {
	case POSITION_NORMAL_TEX:
		glVertexAttribPointer(tex_index_, 2, GL_FLOAT, GL_FALSE, vertex_size_, (GLvoid*)texture_offset_);
		glEnableVertexAttribArray(tex_index_);
		ExitOnGLError("ERROR: Could not enable attribute texture");
	case POSITION_NORMAL:
		if (normal_index_ != 4294967295) { 
			glVertexAttribPointer(normal_index_, 3, GL_FLOAT, GL_FALSE, vertex_size_, (GLvoid*)normal_offset_);
			glEnableVertexAttribArray(normal_index_);
			ExitOnGLError("ERROR: Could not enable attribute normal");
		}
	case POSITION:
		glVertexAttribPointer(position_index_, 3, GL_FLOAT, GL_FALSE, vertex_size_, (GLvoid*)0);
		glEnableVertexAttribArray(position_index_);
		ExitOnGLError("ERROR: Could not enable attribute position");
		break;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
	glDrawElements(draw_mode_, index_count_, GL_UNSIGNED_INT, (GLvoid*)0);
	ExitOnGLError("ERROR: Could not draw the mesh");

	switch(vertex_type_) {
	case POSITION_NORMAL_TEX:
		glDisableVertexAttribArray(tex_index_);
	case POSITION_NORMAL:
		glDisableVertexAttribArray(normal_index_);
	case POSITION:
		glDisableVertexAttribArray(position_index_);
		break;
	}
	//
	//
	
	//glBindVertexArray(0);
};