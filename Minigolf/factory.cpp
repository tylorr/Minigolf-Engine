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

namespace {
	vector<vec3> Square(const float &width, const float &height) {
		vector<vec3> vertex_list;
		float hw = width / 2.0f;
		float hh = height / 2.0f;

		vertex_list.push_back(vec3(hw, 0, -hh));
		vertex_list.push_back(vec3(-hw, 0, -hh));
		vertex_list.push_back(vec3(-hw, 0, hh));
		vertex_list.push_back(vec3(hw, 0, hh));	

		return vertex_list;
	}
}; // namespace

shared_ptr<Geometry> Planar(const GLuint &program, const vector<vec3> &vertex_list);

// returns root entity, used to rotate entire scene
boost::shared_ptr<Entity> CreateLevel(const Hole &hole) {
	vector<Tile>::const_iterator it, ite;

	// todo: Create light entity/component
	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();
	// todo: build material

	shared_ptr<Entity> root = EntityManager::Create();
	shared_ptr<Transform> transform(new Transform());
	EntityManager::AddComponent(root, transform);
	EntityManager::Register(root, "Root");

	CreateBall(hole.tee, transform);
	CreateTee(hole.tee, transform);
	CreateCup(hole.cup, transform);

	for (it = hole.tiles.begin(), ite = hole.tiles.end(); it != ite; ++it) {
		CreateTile(*it, transform, material);
	}

	return root;
}

shared_ptr<Entity> CreateCamera(const float &fov, const float &aspect, const float &near_plane, const float &far_plane) {
	shared_ptr<Entity> entity = EntityManager::Create();

	shared_ptr<Camera> camera(new Camera());
	camera->orthographic = false;
	camera->field_of_view = fov;
	camera->aspect_ratio = aspect;
	camera->near_plane = near_plane;
	camera->far_plane = far_plane;

	shared_ptr<Transform> transform(new Transform());
	transform->position = vec3(0, 1, 3);

	EntityManager::AddComponent(entity, camera);
	EntityManager::AddComponent(entity, transform);
	EntityManager::Register(entity, "Camera");

	return entity;
}

boost::shared_ptr<Entity> CreateTile(const Tile &tile, const boost::shared_ptr<Transform> &root, const boost::shared_ptr<Material> &material) {
	shared_ptr<Geometry> geometry = Planar(material->shader_program(), tile.vertices);

	// build the mesh
	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->parent = root;

	// this causes massive memory leak can't figure out why vector
	// does not release shared_ptr memory
	//root->children.push_back(transform);

	shared_ptr<Entity> entity = EntityManager::Create();

	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	return entity;
}

boost::shared_ptr<Entity> CreateBall(const TeeCup &tee, const boost::shared_ptr<Transform> &root) {
	vector<vec3> vertex_list = Square(0.25f, 0.25f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->position = tee.position;
	transform->position.y += 0.04f;
	transform->parent = root;

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);
	EntityManager::Register(entity, "Ball");

	return entity;
}

boost::shared_ptr<Entity> CreateTee(const TeeCup &tee, const boost::shared_ptr<Transform> &root) {
	vector<vec3> vertex_list = Square(0.25f, 0.25f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->position = tee.position;
	transform->position.y += 0.01f;
	transform->parent = root;

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	return entity;
}
boost::shared_ptr<Entity> CreateCup(const TeeCup &cup, const boost::shared_ptr<Transform> &root) {
	vector<vec3> vertex_list = Square(0.5f, 0.5f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->position = cup.position;
	transform->position.y += 0.01f;
	transform->parent = root;

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	return entity;
}

// Return geometry given counter-clockwise set of convex, co-planar points
shared_ptr<Geometry> Planar(const GLuint &program, const vector<vec3> &vertex_list) {
	Vertex *vertices;
	GLuint *indices;

	vec3 normal;
	vec3 vertex;
	GLsizei N;
	int i, index, count, vertex_index;

	N = vertex_list.size();
	
	vertices = new Vertex[N];
	indices = new GLuint[N];

	// create normal using first 3 points
	normal = glm::normalize(glm::cross(vertex_list[2] - vertex_list[1], vertex_list[0] - vertex_list[1]));

	int blah = 3;

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
	geometry->Initialize(program, POSITION_NORMAL, vertices, N, indices, N);

	delete [] vertices;
	delete [] indices;

	return geometry;
}

}; // namespace Factory