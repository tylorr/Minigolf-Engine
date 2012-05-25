#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"

class Entity;

class CameraController : public EntitySystem {
public:
	CameraController();
	~CameraController();

	virtual void Init();
	virtual void Resolve();
	virtual void Process();
	

private:
	struct CameraProfile {
		glm::vec3 reference;
		glm::vec3 up;
	};

	boost::shared_ptr<Entity> camera_;
	boost::shared_ptr<Entity> ball_;

	bool free_;
	CameraProfile profile_;
	CameraProfile third_person_;
	CameraProfile top_down_;
};

#endif // CAMERA_CONTROLLER_H