#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "physics_system.h"
#include "entity_manager.h"
#include "transform.h"
#include "input.h"
#include "ball_component.h"
#include "tile_component.h"
#include "volume.h"
#include "Utils.h"

using boost::shared_ptr;

PhysicsSystem::PhysicsSystem() : EntitySystem("PhysicsSystem") {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Init(){
	ball_ = EntityManager::Find("Ball");
}

void PhysicsSystem::Resolve(){
}

void PhysicsSystem::Process(){

	shared_ptr<Transform> ball_transform = EntityManager::GetComponent<Transform>(ball_, "Transform");
	//shared_ptr<BallComponent> ball_comp = EntityManager::GetComponent<BallComponent>(ball_, "BallComponent");
	//shared_ptr<TileComponent> curr_tile = EntityManager::GetComponent<TileComponent>(ball_comp->current_tile, "TileComponent");
	//shared_ptr<Volume> volume_comp = EntityManager::GetComponent<Volume>(ball_comp->current_tile, "Volume");
	
	GetVolumes();
	UpdateTile(ball_transform);
	
	//clear vectors at the end so they are current each cycle
	tile_vols_.clear();
	wall_vols_.clear();
	curr_tile = shared_ptr<TileComponent>();
}

void PhysicsSystem::GetVolumes()
{
	//get needed components
	shared_ptr<BallComponent> ball_comp = EntityManager::GetComponent<BallComponent>(ball_, "BallComponent");
	curr_tile = EntityManager::GetComponent<TileComponent>(ball_comp->current_tile, "TileComponent");
	
	//pushes the current tile volume onto vector
	tile_vols_.push_back(EntityManager::GetComponent<Volume>(ball_comp->current_tile, "Volume")); 
	
	//pushes all current tile walls onto vector
	for( unsigned j=0;j<curr_tile->walls.size();j++)  
	{
		shared_ptr<Volume> wall_v = EntityManager::GetComponent<Volume>(curr_tile->walls.at(j), "Volume");
		wall_vols_.push_back(wall_v);
	}

	//push neighbors tile and wall volumes
	for(unsigned i = 0; i < curr_tile->neighbors.size(); i++)
	{
		//push all neighboring tile volumes onto vector
		shared_ptr<Volume> tile_v = EntityManager::GetComponent<Volume>(curr_tile->neighbors.at(i), "Volume"); 
		tile_vols_.push_back(tile_v);
	
		//pushes walls of all neighbors
		shared_ptr<TileComponent> neighbor = EntityManager::GetComponent<TileComponent>(curr_tile->neighbors.at(i), "TileComponent"); 
		for (unsigned j = 0; j < neighbor->walls.size(); j++)
		{
			shared_ptr<Volume> wall_v = EntityManager::GetComponent<Volume>(neighbor->walls.at(j), "Volume");
			wall_vols_.push_back(wall_v);
		}
	}
}

void PhysicsSystem::UpdateTile(const boost::shared_ptr<Transform> &ball_transform) {
	using glm::vec2; 

	float radius = 0.05f;

	shared_ptr<Volume> curr_volume = tile_vols_[0];

	shared_ptr<BallComponent> ball_comp = EntityManager::GetComponent<BallComponent>(ball_, "BallComponent");

	// move ball  up slopes using projections
	vec3 proj = Project(ball_transform->position(), curr_volume->normal, curr_volume->vertices[0]);
	ball_transform->set_position(proj);
	ball_transform->Translate(curr_volume->normal * radius);

	// loop through neighbors
	for (int i = 1, size = tile_vols_.size(); i < size; ++i) {
		shared_ptr<Volume> neigh = tile_vols_[i];

		// project neighbors vertices to xz plane
		vector<vec2> projected_vertices;
		for (int j = 0, sizej = neigh->vertices.size(); j < sizej; ++j) {
			vec3 v = neigh->vertices[j];
			vec2 p(v.x, v.z);

			projected_vertices.push_back(p);
		}

		// project ball to xz plane
		vec2 point(ball_transform->position().x, ball_transform->position().z);

		// check to see if ball overlaps neighbor
		bool inter = PointInPolygon(point, projected_vertices);

		// does ball overlap?
		if (inter) {
			// set current ball to overlapped neighbor
			ball_comp->current_tile = curr_tile->neighbors[i - 1];
			break;
		}
	}
}