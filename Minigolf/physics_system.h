#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include "glm\glm.hpp"
#include "entity_system.h"

class ComponentType;
struct TileComponent;
struct Volume;
struct Transform;

class PhysicsSystem : public EntitySystem {
public:
	void Init();
	void Resolve();
	void Process();
	void GetVolumes();

	PhysicsSystem();
	~PhysicsSystem();
private:
	boost::shared_ptr<Entity> ball_;
	std::vector<boost::shared_ptr<Volume>> tile_vols_;
	std::vector<boost::shared_ptr<Volume>> wall_vols_;
	boost::shared_ptr<TileComponent> curr_tile;

	void UpdateTile(const boost::shared_ptr<Transform> &ball_transform);
};

#endif