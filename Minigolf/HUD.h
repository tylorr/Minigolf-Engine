#ifndef HUD_H
#define HUD_H

#include <vector>

#include <boost\shared_ptr.hpp>

#include "component.h"
#include "entity.h"
#include "glm\glm.hpp"

struct hud_element{
	string name;
	glm::vec3 position;
}

struct HUD : public Component {
	std::vector<hud_element> hud_elements;
};

typedef boost::shared_ptr<HUD> HUDPtr;

#endif // HUD_H