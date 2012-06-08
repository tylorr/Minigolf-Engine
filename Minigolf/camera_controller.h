#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "entity_system.h"

class Entity;

class CameraController : public EntitySystem {
public:
	CameraController(const int &layer);
	CameraController(const int &layer, const std::string &script);
	~CameraController();

	virtual void Init();
	virtual void ReloadScript();
	virtual void Process();
	

private:
	struct CameraProfile {
		glm::vec3 reference;
		glm::vec3 up;
	};

	bool free_;
	CameraProfile *profile_;
	CameraProfile third_person_;
	CameraProfile top_down_;

	float head_degrees_;
	float pitch_degrees_;
};

#endif // CAMERA_CONTROLLER_H