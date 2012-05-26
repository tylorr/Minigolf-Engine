#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <boost\shared_ptr.hpp>
#include "glm\glm.hpp"
#include "entity_system.h"
#include "volume.h"

class ComponentType;
struct TileComponent;

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
	std::vector<boost::shared_ptr<Volume>> wall_vols_;
	boost::shared_ptr<TileComponent> curr_tile;

};

#endif