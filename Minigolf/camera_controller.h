#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <boost\shared_ptr.hpp>

#include "entity_system.h"

class Entity;
struct Transform;

class CameraController : public EntitySystem {
public:
	CameraController();
	~CameraController();

	virtual void Process();
	virtual void Resolve();

private:
	boost::shared_ptr<Transform> camera_transform_;
	boost::shared_ptr<Transform> ball_transform_;
};

#endif // CAMERA_CONTROLLER_H