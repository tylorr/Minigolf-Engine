#ifndef THIRD_PERSON_CAMERA_SYSTEM
#define THIRD_PERSON_CAMERA_SYSTEM

#include "entity_system.h"

class ComponentType;
struct Transform;
struct Camera;

class ThirdPersonCameraSystem : public EntitySystem {
public:
	ThirdPersonCameraSystem();
	~ThirdPersonCameraSystem();

	virtual void Process();
};

#endif // THIRD_PERSON_CAMERA_SYSTEM