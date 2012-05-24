#ifndef RENDERER_H
#define RENDERER_H

	#include <boost\shared_ptr.hpp>

	#include "glm\glm.hpp"

	#include "entity_system.h"

	class ComponentType;
struct Transform;
struct Camera;

class RenderSystem : public EntitySystem {
public:
	bool relative_;
	glm::vec3 reference_;
	glm::vec3 up_;

	RenderSystem(const bool &relative, const glm::vec3 &reference, const glm::vec3 &up);
	~RenderSystem();

	virtual void ProcessEntities(const EntityMap &entities);

	//virtual bool CheckEntity(const bool &interest, const bool &contains, const boost::shared_ptr<Entity> &entity);

private:
	boost::shared_ptr<ComponentType> mesh_type_;
	boost::shared_ptr<ComponentType> transform_type_;
	
};

#endif // RENDERER_H