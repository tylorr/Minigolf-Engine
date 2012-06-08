#ifndef GUI_TEXT_RENDER_H
#define GUI_TEXT_RENDER_H

#include "entity_system.h"
#include "component_mapper.h"

struct GuiText;

class GuiTextRender : public EntitySystem {
public:
	GuiTextRender(const int &layer);
	~GuiTextRender();

	virtual void ProcessEntities(const EntityMap &entities);
	
private:
	ComponentMapper<GuiText> text_mapper_;
};

#endif // GUI_TEXT_RENDER_H