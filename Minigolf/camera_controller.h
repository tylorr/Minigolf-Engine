#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <boost\shared_ptr.hpp>

#include "entity_system.h"

class Entity;

class CameraController : public EntitySystem {
public:
	CameraController();
	~CameraController();

	virtual void Resolve();
	virtual void Process();
	

private:
	boost::shared_ptr<Entity> camera_;
	boost::shared_ptr<Entity> ball_;
};

#endif // CAMERA_CONTROLLER_H