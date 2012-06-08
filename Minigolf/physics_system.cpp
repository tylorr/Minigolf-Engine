#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
#include "lua.hpp"
#include "luabind\luabind.hpp"

#include "physics_system.h"
#include "entity_manager.h"
#include "transform.h"
#include "input.h"
#include "ball_component.h"
#include "tile_component.h"
#include "volume.h"
#include "Utils.h"
#include "time.h"
#include "mesh.h"
#include "basic_material.h"
#include "volume.h"


using boost::shared_ptr;
using boost::unordered_map;

using glm::vec3;

PhysicsSystem::PhysicsSystem(const int &layer, const std::string &script) : EntitySystem(layer, script) { }

PhysicsSystem::~PhysicsSystem() { }

void PhysicsSystem::Init(){
	EntitySystem::Init();

	ball_ = EntityManager::Find("Ball");
	friction_ = 0.985f;
	gravity_ = 15.f;
}

void PhysicsSystem::Resolve(){
}

void PhysicsSystem::ReloadScript() {
	EntitySystem::ReloadScript();
	
	friction_ = GetAttribute<float>("friction");
}

void PhysicsSystem::Process(){
	ball_ = EntityManager::Find("Ball");

	if (!ball_) {
		return;
	}

	TransformPtr ball_transform = transform_mapper_(ball_);
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);

	MeshPtr mesh = mesh_mapper_(ball_comp->current_tile);
	shared_ptr<BasicMaterial> bm = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);
	bm->Ld_ = vec3(1, 1, 0);
	
	//shared_ptr<TileComponent> curr_tile = EntityManager::GetComponent<TileComponent>(ball_comp->current_tile, "TileComponent");
	//shared_ptr<Volume> volume_comp = EntityManager::GetComponent<Volume>(ball_comp->current_tile, "Volume");
	float delta = Time::GetDeltaTime();

	GetVolumes();
	UpdateTile(ball_transform);
	UpdateCollision(ball_transform);
	ApplyGravity();
	ApplyFriction();

	TileComponentPtr curr_tile = tile_comp_mapper_(ball_comp->current_tile);
	if(curr_tile->cup){
		//std::cout<<"dont break"<<std::endl;
		CheckCup(ball_transform);
	}

	float epsilon = 0.2f;

	ball_comp->velocity += ball_comp->acceleration * delta;
	ball_transform->Translate(ball_comp->velocity * delta);

	if (glm::length(ball_comp->acceleration) < epsilon) {
		ball_comp->acceleration = vec3(0);
	}

	if (glm::length(ball_comp->velocity) < epsilon) {
		ball_comp->velocity = vec3(0);
	}

	if (glm::length(ball_comp->velocity) > 0) {
		vec3 target = ball_comp->velocity;
		target.y = 0;
		target += ball_transform->position();
		ball_transform->LookAt(target);
	}
	
	//clear vectors at the end so they are current each cycle
	tile_vols_.clear();
	wall_map_.clear();
	curr_tile = TileComponentPtr();
}

void PhysicsSystem::GetVolumes()
{
	//get needed components
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);
	curr_tile = tile_comp_mapper_(ball_comp->current_tile);
	
	//pushes the current tile volume onto vector
	tile_vols_.push_back(volume_mapper_(ball_comp->current_tile)); 
	
	//pushes all current tile walls onto vector
	for( unsigned j=0;j<curr_tile->walls.size();j++)  
	{
		VolumePtr wall_v = volume_mapper_(curr_tile->walls.at(j));
		//wall_vols_.push_back(wall_v);
		wall_map_[wall_v] = curr_tile->walls[j];
	}

	//push neighbors tile and wall volumes
	for(unsigned i = 0; i < curr_tile->neighbors.size(); i++)
	{
		//push all neighboring tile volumes onto vector
		VolumePtr tile_v = volume_mapper_(curr_tile->neighbors.at(i)); 
		tile_vols_.push_back(tile_v);
	
		//pushes walls of all neighbors
		TileComponentPtr neighbor = tile_comp_mapper_(curr_tile->neighbors.at(i)); 
		for (unsigned j = 0; j < neighbor->walls.size(); j++)
		{
			VolumePtr wall_v = volume_mapper_(neighbor->walls.at(j));
			//wall_vols_.push_back(wall_v);
			wall_map_[wall_v] = neighbor->walls[j];
		}
	}
}

void PhysicsSystem::UpdateTile(const TransformPtr &ball_transform) {
	using glm::vec2; 

	float radius = 0.05f;

	VolumePtr curr_volume = tile_vols_[0];

	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);

	// move ball  up slopes using projections
	vec3 proj = Project(ball_transform->position(), curr_volume->normal, curr_volume->vertices[0]);
	ball_transform->set_position(proj);
	ball_transform->Translate(curr_volume->normal * radius);

	// loop through neighbors
	for (int i = 1, size = tile_vols_.size(); i < size; ++i) {
		VolumePtr neigh = tile_vols_[i];

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
			MeshPtr mesh = mesh_mapper_(ball_comp->current_tile);
			shared_ptr<BasicMaterial> bm = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);
			bm->Ld_ = vec3(0, 1, 0);

			ball_comp->current_tile = curr_tile->neighbors[i - 1];
		
			break;
		}
	}
}

void PhysicsSystem::CheckCup(const TransformPtr &ball_transform){
	using glm::vec2; 

	float radius = 0.05f;

	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);
	TileComponentPtr curr_tile = tile_comp_mapper_(ball_comp->current_tile);
	VolumePtr cup_vol = volume_mapper_(curr_tile->cup);
	TransformPtr cup_transform = transform_mapper_(curr_tile->cup);

	// create ball projection
	vec3 proj = Project(ball_transform->position(), cup_vol->normal, cup_vol->vertices[0]);

	// project cup vertices to xz plane
	vector<vec2> projected_vertices;
	for (int j = 0, sizej = cup_vol->vertices.size(); j < sizej; ++j) {
		vec3 v = cup_vol->vertices[j];
		vec2 p(v.x, v.z);

		projected_vertices.push_back(p);
	}

	// project ball to xz plane
	vec2 point(proj.x, proj.z);

	// check to see if ball overlaps hole
	bool inter = PointInPolygon(point, projected_vertices);

	// does ball overlap?
	if (inter) {
		float dist_from_cup = glm::length(ball_transform->position() - cup_transform->position());

		//if ball is close enough to go in hole and moving slow enough, stop ball
		if(dist_from_cup < MADE_CUP_RADIUS && glm::length(ball_comp->velocity) < MAX_CUP_ENTRY_SPEED ) {
			ball_comp->velocity = vec3();
			ball_comp->acceleration = vec3();
		}
		//if ball is on lip, alter accel to go towards center
		else if(dist_from_cup > MADE_CUP_RADIUS && dist_from_cup < CUP_LIP_RADIUS) {
			ball_comp->acceleration += glm::normalize(cup_transform->position() - ball_transform->position()) * LIP_ACCEL;
		}
		else{}
	}
}

void PhysicsSystem::ApplyFriction(){
	//grab ball component and dampen velocity based on coefficient of friction
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);
	//ball_comp->velocity *= friction_;
	vec3 dir = -glm::normalize(ball_comp->velocity);
	float force = glm::length(gravity_) * friction_;

	if (glm::length(ball_comp->velocity) != 0) {
		ball_comp->acceleration += force * dir;
	}
}

void PhysicsSystem::ApplyGravity(){
	//grab ball component
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);

	//x is parallel x vector for current tile, r is downward slope vector
	glm::vec3 x;
	glm::vec3 r;

	//calculate x and r
	x = glm::cross(glm::vec3(0,1,0), tile_vols_[0]->normal);
	r = glm::cross(x, tile_vols_[0]->normal);

	float delta = Time::GetDeltaTime();
	if(glm::length(r)>0){ glm::normalize(r); }
	//ball_comp->velocity += r * gravity_ * delta;
	ball_comp->acceleration += (r * gravity_);
}

void PhysicsSystem::UpdateCollision(const TransformPtr &ball_transform) {
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);
	vec3 start = ball_transform->position();
	vec3 end = start + ball_comp->velocity * Time::GetDeltaTime();

	vec3 normal, penetration;

	unordered_map<VolumePtr, EntityPtr>::iterator it;
	for (it = wall_map_.begin(); it != wall_map_.end(); ++it) {
		if (Intersect(start, end, it->first, normal, penetration)) {
			ResolveCollision(ball_transform, normal, penetration);
		}
	}
}

bool PhysicsSystem::Intersect(const vec3 &start, const vec3 &end, const VolumePtr &wall, vec3 &normal, vec3 &penetration) {
	using glm::vec2;

	static const unsigned int PLANE_FRONT = 0;
	static const unsigned int PLANE_BACK = 1;
	static const unsigned int ON_PLANE = 2;

	float radius = 0.05f;
	
	float p;
	vec3 n = wall->normal;
	vec3 offset = vec3(0); //n * radius; // This hack for adding radius to ball does not work when walls do not form convex shape
	float d = -glm::dot(n, (wall->vertices[0] + offset));
	unsigned int start_loc = 3;
	unsigned int end_loc = 3;
	
	p = glm::dot(n, start) + d;
	if (p > 0.0f) {
		start_loc = PLANE_FRONT;
	} else if (p < 0.0f) {
		start_loc = PLANE_BACK;
	} else  {
		start_loc = ON_PLANE;
	}

	p = glm::dot(n, end) + d;
	if (p > 0.0f) {
		end_loc = PLANE_FRONT;
	} else if (p < 0.0f) {
		end_loc = PLANE_BACK;
	} else  {
		end_loc = ON_PLANE;
	}

	/*
	This hack only works if the walls form a convex shape
	// HACK: tries to prevent ball from ever being behind a wall
	if (start_loc == PLANE_BACK) {
		TransformPtr ball_transform = transform_mapper_(ball_);
		BallComponentPtr ball_comp = ball_comp_mapper_(ball_);
		vec3 new_start = start - ball_comp->velocity * Time::GetDeltaTime();
		ball_transform->set_position(new_start);
		return false;
	}
	*/

	MeshPtr mesh;// = EntityManager::GetComponent<Mesh>(wall_map_[wall], "Mesh");
	shared_ptr<BasicMaterial> bm;// = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);
	//bm->Ld_ = vec3(1, 0, 0);

	if (start_loc == end_loc) {
		return false;
	}

	mesh = mesh_mapper_(wall_map_[wall]);
	bm = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);
	bm->Ld_ = vec3(1, 0, 1);

	vec3 ray = end - start;
	ray = glm::normalize(ray);

	float t = - (d + glm::dot(n, start)) / glm::dot(n, ray);

	vec3 intersect = start + (t * ray);

	bool x_axis = false;

	if (glm::abs(n.x) < glm::abs(n.z)) {
		x_axis = true;
	}

	vec2 pos;

	if (x_axis) {
		pos = vec2(intersect.x, intersect.y);
	} else  {
		pos = vec2(intersect.z, intersect.y);
	}

	vec2 vert;
	vector<vec2> vertices;
	vector<vec3>::const_iterator it;
	for (it = wall->vertices.begin(); it != wall->vertices.end(); ++it) {
		if (x_axis) {
			vert = vec2(it->x, it->y);
			vert += vec2(offset.x, offset.y);
			vertices.push_back(vert);
		} else {
			vert = vec2(it->z, it->y);
			vert += vec2(offset.z, offset.y);
			vertices.push_back(vert);
		}
	}

	normal = n;
	penetration = intersect;
	bool result = PointInPolygon(pos, vertices);

	if (result) {
		mesh = mesh_mapper_(wall_map_[wall]);
		bm = boost::dynamic_pointer_cast<BasicMaterial>(mesh->material);
		bm->Ld_ = vec3(1, 1, 1);
	}

	return result;
}

void PhysicsSystem::ResolveCollision(const TransformPtr &ball_transform, const vec3 &normal, const vec3 &intersection) {
	BallComponentPtr ball_comp = ball_comp_mapper_(ball_);

	ball_transform->set_position(intersection);

	vec3 direction = ball_comp->velocity;
	vec3 w = glm::dot(normal, -direction) * normal;
	vec3 result = w + (w + direction);
	ball_comp->velocity = result;
}