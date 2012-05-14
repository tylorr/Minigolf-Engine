#include "GL\glew.h"
#include "GL\freeglut.h"

#include "factory.h"
//#include "renderer.h"
#include "entity.h"
#include "camera.h"
#include "mesh.h"
#include "material.h"

namespace Factory {

/*
Entity *CreateTile(Camera *camera, Material *material, const std::vector<glm::vec3> &vertex_list) {	
	Entity *entity;
	Mesh *mesh;
	//Renderer *renderer;
	Vertex *vertices;
	GLuint *indices;
	vec3 normal;
	glm::vec3 vertex;
	GLsizei N;
	int i, index, count, vertex_index;

	N = vertex_list.size();
	
	vertices = new Vertex[N];
	indices = new GLuint[N];

	// create normal using first 3 points
	normal = glm::normalize(glm::cross(vertex_list[2] - vertex_list[1], vertex_list[0] - vertex_list[1]));

	// build the vertices
	for (i = 0; i < N; ++i) {
		vertex = vertex_list[i];

		vertices[i].Position[0] = vertex.x;
		vertices[i].Position[1] = vertex.y;
		vertices[i].Position[2] = vertex.z;

		vertices[i].Normal[0] = normal.x;
		vertices[i].Normal[1] = normal.y;
		vertices[i].Normal[2] = normal.z;
	}

	// build the indices
	index = count = 0;
	while (index < N)
	{
		switch (index % 2) {
		case 0:
			vertex_index = (N - count) % N;
			break;
		case 1:
			count++;
			vertex_index = count;
			break;
		}
		indices[index] = vertex_index;
		index++;
	}

	// build the mesh
	mesh = new Mesh();
	mesh->Initialize(POSITION_NORMAL, vertices, N, indices, N);

	// build render component
	//renderer = new Renderer(camera, mesh, material);

	// build entity
	entity = new Entity();
	//entity->AddComponent(renderer);

	return entity;
}
*/

}; // namespace Factory