#ifndef RENDERER_H
#define RENDERER_H

#include <boost\shared_ptr.hpp>

#include "entity_system.h"

class ComponentType;
struct Transform;
struct Camera;

class RenderSystem : public EntitySystem {
public:
	RenderSystem();
	~RenderSystem();

	virtual void ProcessEntities(const EntityMap &entities);

	//virtual bool CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity);

private:
	boost::shared_ptr<ComponentType> mesh_type_;
	boost::shared_ptr<ComponentType> transform_type_;
	//boost::shared_ptr<ComponentType> camera_type_;

	//boost::shared_ptr<Transform> camera_transform_;
	//boost::shared_ptr<Camera> camera_;

	long camera_bits_;
};

#endif // RENDERER_H