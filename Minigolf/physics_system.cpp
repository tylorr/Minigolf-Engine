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
	
	//clear vectors at the end so they are current each cycle
	tile_vols_.clear();
	wall_vols_.clear();
}

void PhysicsSystem::GetVolumes()
{
	//get needed components
	shared_ptr<BallComponent> ball_comp = EntityManager::GetComponent<BallComponent>(ball_, "BallComponent");
	shared_ptr<TileComponent> curr_tile = EntityManager::GetComponent<TileComponent>(ball_comp->current_tile, "TileComponent");
	
	//pushes the current tile volume onto vector
	tile_vols_.push_back(EntityManager::GetComponent<Volume>(ball_comp->current_tile, "Volume")); 
	
	//pushes all current tile walls onto vector
	for( unsigned j=0;j<curr_tile->walls.size();j++)  
	{
		shared_ptr<Volume> wall_v = EntityManager::GetComponent<Volume>(curr_tile->walls.at(j), "Volume");
		wall_vols_.push_back(wall_v);
	}

	//push neighbors tile and wall volumes
	for(unsigned i=0;i<curr_tile->neighbors.size();i++)
	{
		//push all neighboring tile volumes onto vector
		shared_ptr<Volume> tile_v = EntityManager::GetComponent<Volume>(curr_tile->neighbors.at(i), "Volume"); 
		tile_vols_.push_back(tile_v);
	
		//pushes walls of all neighbors
		shared_ptr<TileComponent> neighbor = EntityManager::GetComponent<TileComponent>(curr_tile->neighbors.at(i), "TileComponent"); 
		for( unsigned j=0;j<neighbor->walls.size();j++)
		{
			shared_ptr<Volume> wall_v = EntityManager::GetComponent<Volume>(neighbor->walls.at(i), "Volume");
			wall_vols_.push_back(wall_v);
		}
	}
}