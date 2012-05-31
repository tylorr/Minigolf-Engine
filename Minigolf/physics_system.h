#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"
#include "component_mapper.h"

class Entity;
class ComponentType;
struct Transform;
struct TileComponent;
struct Volume;
struct Mesh;
struct BallComponent;

class PhysicsSystem : public EntitySystem {
public:
	PhysicsSystem(const int &layer);
	~PhysicsSystem();

	void Init();
	void Resolve();
	void Process();
private:
	float gravity_;
	float friction_;

	ComponentMapper<Transform> transform_mapper_;
	ComponentMapper<Volume> volume_mapper_;
	ComponentMapper<Mesh> mesh_mapper_;
	ComponentMapper<BallComponent> ball_comp_mapper_;
	ComponentMapper<TileComponent> tile_comp_mapper_;

	boost::shared_ptr<Entity> ball_;
	std::vector<boost::shared_ptr<Volume>> tile_vols_;
	//std::vector<boost::shared_ptr<Volume>> wall_vols_;
	boost::unordered_map<boost::shared_ptr<Volume>, boost::shared_ptr<Entity>> wall_map_;
	boost::shared_ptr<TileComponent> curr_tile;

	void UpdateTile(const boost::shared_ptr<Transform> &ball_transform);

	void GetVolumes();
	void ApplyFriction();
	void ApplyGravity();

	void UpdateCollision(const boost::shared_ptr<Transform> &ball_transform);
	bool Intersect(const glm::vec3 &start, const glm::vec3 &end, const boost::shared_ptr<Volume> &wall, glm::vec3 &normal, glm::vec3 &penetration);
	void ResolveCollision(const boost::shared_ptr<Transform> &ball_transform, const glm::vec3 &normal, const glm::vec3 &penetration);
};

#endif