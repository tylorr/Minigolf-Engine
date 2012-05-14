#include <vector>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "Utils.h"
#include "factory.h"
#include "entity_manager.h"
#include "entity.h"
#include "camera.h"
#include "transform.h"
#include "material.h"
#include "basic_material.h"
#include "geometry.h"
#include "mesh.h"

namespace Factory {

using boost::shared_ptr;
using glm::vec3;
using glm::vec4;
using std::vector;

shared_ptr<Entity> CreateCamera(const float &fov, const float &aspect, const float &near_plane, const float &far_plane) {
	shared_ptr<Entity> entity = EntityManager::Create();

	shared_ptr<Camera> camera(new Camera());
	camera->orthographic = false;
	camera->field_of_view = fov;
	camera->aspect_ratio = aspect;
	camera->near_plane = near_plane;
	camera->far_plane = far_plane;

	shared_ptr<Transform> transform(new Transform());
	transform->position = vec3(0, 0, 4);

	EntityManager::AddComponent(entity, camera);
	EntityManager::AddComponent(entity, transform);

	return entity;
}

void CreateLevel(const Hole &hole) {
	vector<Tile>::const_iterator it, ite;

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0, 5.0f, 0, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();
	// todo: build material

	for (it = hole.tiles.begin(), ite = hole.tiles.end(); it != ite; ++it) {
		CreateTile(*it, material);
	}
}

void CreateTile(const Tile &tile, boost::shared_ptr<Material> material) {
	Vertex *vertices;
	GLuint *indices;

	vec3 normal;
	vec3 vertex;
	GLsizei N;
	int i, index, count, vertex_index;

	vector<vec3> vertex_list = tile.vertices;

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
	shared_ptr<Geometry> geometry(new Geometry());
	geometry->Initialize(POSITION_NORMAL, vertices, N, indices, N);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());

	shared_ptr<Entity> entity = EntityManager::Create();

	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	delete [] vertices;
	delete [] indices;
}

}; // namespace Factory