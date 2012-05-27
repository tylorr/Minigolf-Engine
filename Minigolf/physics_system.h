#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "volume.h"
#include "tile_component.h"
#include "transform.h"

class Entity;
class ComponentType;

using glm::vec3;
using boost::unordered_map;
using std::vector;
using boost::shared_ptr;

class PhysicsSystem : public EntitySystem {
public:
	void Init();
	void Resolve();
	void Process();
	void GetVolumes();
	void ApplyFriction();
	void ApplyGravity();

	PhysicsSystem();
	~PhysicsSystem();
private:
	float gravity_;
	float friction_;
	EntityPtr ball_;
	std::vector<VolumePtr> tile_vols_;
	//std::vector<boost::shared_ptr<Volume>> wall_vols_;
	unordered_map<VolumePtr, EntityPtr> wall_map_;
	TileComponentPtr curr_tile;

	void UpdateTile(const TransformPtr &ball_transform);

	void UpdateCollision(const TransformPtr &ball_transform);
	bool Intersect(const vec3 &start, const vec3 &end, const VolumePtr &wall, glm::vec3 &normal, glm::vec3 &penetration);
	void ResolveCollision(const TransformPtr &ball_transform, const glm::vec3 &normal, const glm::vec3 &penetration);
};

#endif