#ifndef HUD_H
#define HUD_H

#include "component.h"
#include "entity.h"
#include "glm\glm.hpp"

struct hud_element{
	string name;
	glm::vec3 position;
}

struct HUD : public Component {

	vector<hud_element> hud_elements;
};

#endif