#include <vector>

#include <boost\unordered_map.hpp>

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
#include "volume.h"
#include "tile_component.h"

namespace Factory {

using boost::shared_ptr;
using glm::vec3;
using glm::vec4;
using std::vector;
using boost::unordered_map;

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

	vec3 GetNormal(const vector<vec3> &vertex_list) {
		return glm::normalize(glm::cross(vertex_list[2] - vertex_list[1], vertex_list[0] - vertex_list[1]));
	}
}; // namespace

// returns root entity, used to rotate entire scene
void CreateLevel(const Hole &hole) {
	vector<Tile>::const_iterator it, ite;

	// todo: Create light entity/component
	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();
	// todo: build material

	CreateBall(hole.tee);
	CreateTee(hole.tee);
	shared_ptr<Entity> cup = CreateCup(hole.cup);

	unordered_map<int, shared_ptr<Entity>> tiles;

	// building tiles and their walls
	for (it = hole.tiles.begin(), ite = hole.tiles.end(); it != ite; ++it) {
		tiles[it->id] = CreateTile(*it, material);
	}

	// build the tile_component
	shared_ptr<Entity> wall;
	shared_ptr<TileComponent> tile_comp;
	for (it = hole.tiles.begin(), ite = hole.tiles.end(); it != ite; ++it) {

		tile_comp = shared_ptr<TileComponent>(new TileComponent());
		for (size_t i = 0; i < it->neighbors.size(); ++i) {
			int id = it->neighbors[i];

			// does it have an empty edge here?
			if (id == 0) {
				// build wall
				size_t j = (i + 1) % it->vertices.size();
				wall = CreateWall(GetNormal(it->vertices), it->vertices[i], it->vertices[j]);
				tile_comp->walls.push_back(wall);
			} else {
				// attach neighbor
				tile_comp->neighbors.push_back(tiles[id]);
			}
		}

		if (hole.cup.id == it->id) {
			tile_comp->has_cup = true;
			tile_comp->cup = cup;
		}

		EntityManager::AddComponent(tiles[it->id], tile_comp);
	}
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
	transform->set_position(vec3(0, 1, 3));

	EntityManager::AddComponent(entity, camera);
	EntityManager::AddComponent(entity, transform);
	EntityManager::Register(entity, "Camera");

	return entity;
}

boost::shared_ptr<Entity> CreateTile(const Tile &tile, const boost::shared_ptr<Material> &material) {
	vec3 normal = GetNormal(tile.vertices);
	shared_ptr<Geometry> geometry = Planar(material->shader_program(), normal, tile.vertices);

	// build the mesh
	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Volume> volume(new Volume());
	volume->vertices = tile.vertices;
	volume->normal = normal;

	shared_ptr<Transform> transform(new Transform());

	shared_ptr<Entity> entity = EntityManager::Create();

	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);
	EntityManager::AddComponent(entity, volume);

	return entity;
}

boost::shared_ptr<Entity> CreateWall(const vec3 &tile_normal, const vec3 &p1, const vec3 &p2) {
	float width = 0.25f;
	float height = 0.5f;

	vec3 forward = glm::normalize(p2 - p1);
	vec3 left = glm::normalize(glm::cross(tile_normal, forward));

	vec3 p3 = p2 - (left * width);
	vec3 p4 = p1 - (left * width);

	vector<vec3> vertex_list;
	vertex_list.push_back(p4);
	vertex_list.push_back(p3);
	vertex_list.push_back(p2);
	vertex_list.push_back(p1);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), tile_normal, vertex_list);

	shared_ptr<Volume> volume(new Volume());

	vec3 h3 = p2 + (tile_normal * height);
	vec3 h4 = p1 + (tile_normal * height);

	volume->vertices.push_back(h4);	
	volume->vertices.push_back(h3);
	volume->vertices.push_back(p2);
	volume->vertices.push_back(p1);

	volume->normal = left;

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);
	EntityManager::AddComponent(entity, volume);

	return entity;
}

boost::shared_ptr<Entity> CreateBall(const TeeCup &tee) {
	vector<vec3> vertex_list = Square(0.25f, 0.25f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), GetNormal(vertex_list), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->set_position(tee.position);
	transform->Translate(0, 0.02f, 0);

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);
	EntityManager::Register(entity, "Ball");

	return entity;
}

boost::shared_ptr<Entity> CreateTee(const TeeCup &tee) {
	vector<vec3> vertex_list = Square(0.25f, 0.25f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), GetNormal(vertex_list), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->set_position(tee.position);
	transform->Translate(0, 0.01f, 0);

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	return entity;
}

boost::shared_ptr<Entity> CreateCup(const TeeCup &cup) {
	vector<vec3> vertex_list = Square(0.5f, 0.5f);

	shared_ptr<BasicMaterial> material(new BasicMaterial("diffuse", vec4(0.0f, 5.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.8f, 0.8f, 0.8f)));
	material->Initialize();

	shared_ptr<Geometry> geometry = Planar(material->shader_program(), GetNormal(vertex_list), vertex_list);

	shared_ptr<Mesh> mesh(new Mesh());
	mesh->geometry = geometry;
	mesh->material = material;

	shared_ptr<Transform> transform(new Transform());
	transform->set_position(cup.position);
	transform->Translate(0, 0.01f, 0);

	shared_ptr<Entity> entity = EntityManager::Create();
	EntityManager::AddComponent(entity, mesh);
	EntityManager::AddComponent(entity, transform);

	return entity;
}

// Return geometry given counter-clockwise set of convex, co-planar points
shared_ptr<Geometry> Planar(const GLuint &program, const vec3 &normal, const vector<vec3> &vertex_list) {
	Vertex *vertices;
	GLuint *indices;

	vec3 vertex;
	GLsizei N;
	int i, index, count, vertex_index;

	N = vertex_list.size();
	
	vertices = new Vertex[N];
	indices = new GLuint[N];

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
	geometry->Initialize(program, GL_TRIANGLE_STRIP, POSITION_NORMAL, vertices, N, indices, N);

	delete [] vertices;
	delete [] indices;

	return geometry;
}

}; // namespace Factory