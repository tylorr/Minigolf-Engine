#ifndef MESH_H
#define MESH_H

#include "Utils.h"

enum VertexType { POSITION, POSITION_NORMAL, POSITION_NORMAL_TEX };

struct Vertex
{
	float Position[3];
	float Normal[3];
	float TexCoord[2];
};

class Mesh {
public:
	void Initialize(const VertexType &vertex_type, const Vertex *vertices, const GLsizei &vertex_count, const GLuint *indices, const GLsizei &index_count);
	void Destroy();

	void Draw() const;
private:
	VertexType vertex_type_;
	GLsizei index_count_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
	GLuint index_buffer_object_;
};

#endif // MESH_H