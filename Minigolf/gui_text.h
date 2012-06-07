#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include <string>

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"

#include "component.h"

struct GuiText : public Component {
	std::string text;
	glm::vec2 position;
};

typedef boost::shared_ptr<GuiText> GUITextPtr;

#endif // GUI_TEXT_H