#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Utils.h"

enum VertexType { POSITION, POSITION_NORMAL, POSITION_NORMAL_TEX };

struct Vertex
{
	float Position[3];
	float Normal[3];
	float TexCoord[2];
};

class Geometry {
public:
	void Initialize(const GLuint &program, const VertexType &vertex_type, const Vertex *vertices, const GLsizei &vertex_count, const GLuint *indices, const GLsizei &index_count);
	void Destroy();

	void Draw() const;
private:
	VertexType vertex_type_;
	GLsizei index_count_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
	GLuint index_buffer_object_;

	GLuint position_index_;
	GLuint normal_index_;
	GLuint tex_index_;

	size_t vertex_size_;
	size_t normal_offset_;
	size_t texture_offset_;
};

#endif // GEOMETRY_H