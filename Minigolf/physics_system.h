#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include <boost\unordered_map.hpp>

#include "glm\glm.hpp"
#include "entity_system.h"
#include "component_mapper.h"

#define LIP_ACCEL			60.0f
#define CUP_LIP_RADIUS		0.3f
#define MAX_CUP_ENTRY_SPEED	10.0f
#define MADE_CUP_RADIUS		0.1f

class Entity;
class ComponentType;
struct Transform;
struct TileComponent;
struct Volume;
struct Mesh;
struct BallComponent;

class PhysicsSystem : public EntitySystem {
public:
	PhysicsSystem(const int &layer, const std::string &script);
	virtual ~PhysicsSystem();

	void Process();

	void ReloadScript();
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

	bool UpdateTile(const boost::shared_ptr<Transform> &ball_transform, const boost::shared_ptr<BallComponent> &ball_comp, const boost::shared_ptr<Entity> &tile, const int &depth);
	void ProjectToSlope(const boost::shared_ptr<Transform> &ball_transform, const boost::shared_ptr<BallComponent> &ball_comp, const boost::shared_ptr<Entity> &tile);
	void CheckCup(const boost::shared_ptr<Transform> &ball_transform);

	void GetVolumes();
	void ApplyFriction();
	void ApplyGravity();

	void UpdateCollision(const boost::shared_ptr<Transform> &ball_transform);
	bool Intersect(const glm::vec3 &start, const glm::vec3 &end, const boost::shared_ptr<Volume> &wall, glm::vec3 &normal, glm::vec3 &penetration);
	void ResolveCollision(const boost::shared_ptr<Transform> &ball_transform, const glm::vec3 &normal, const glm::vec3 &penetration);
};

#endif // PHYSICS_SYSTEM_H