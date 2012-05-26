#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "volume.h"

class Entity;
class ComponentType;
struct TileComponent;
struct Transform;

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
	boost::shared_ptr<Entity> ball_;
	std::vector<boost::shared_ptr<Volume>> tile_vols_;
	//std::vector<boost::shared_ptr<Volume>> wall_vols_;
	unordered_map<shared_ptr<Volume>, shared_ptr<Entity>> wall_map_;
	boost::shared_ptr<TileComponent> curr_tile;

	void UpdateTile(const boost::shared_ptr<Transform> &ball_transform);

	void UpdateCollision(const boost::shared_ptr<Transform> &ball_transform);
	bool Intersect(const vec3 &start, const vec3 &end, const shared_ptr<Volume> &wall, glm::vec3 &normal, glm::vec3 &penetration);
	void ResolveCollision(const boost::shared_ptr<Transform> &ball_transform, const glm::vec3 &normal, const glm::vec3 &penetration);
};

#endif