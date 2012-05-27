#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"

class Entity;
class ComponentType;
struct Transform;
struct TileComponent;
struct Volume;

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
	std::vector<boost::shared_ptr<Volume>> tile_vols_;
	//std::vector<boost::shared_ptr<Volume>> wall_vols_;
	boost::unordered_map<boost::shared_ptr<Volume>, boost::shared_ptr<Entity>> wall_map_;
	boost::shared_ptr<TileComponent> curr_tile;

	void UpdateTile(const boost::shared_ptr<Transform> &ball_transform);

	void UpdateCollision(const boost::shared_ptr<Transform> &ball_transform);
	bool Intersect(const glm::vec3 &start, const glm::vec3 &end, const boost::shared_ptr<Volume> &wall, glm::vec3 &normal, glm::vec3 &penetration);
	void ResolveCollision(const boost::shared_ptr<Transform> &ball_transform, const glm::vec3 &normal, const glm::vec3 &penetration);
};

#endif