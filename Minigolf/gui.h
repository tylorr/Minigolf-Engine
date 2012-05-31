#ifndef GUI_H
#define GUI_H

#include "entity_system.h"
#include "component_mapper.h"

struct GUIText;

class GUI : public EntitySystem {
public:
	GUI(const int &layer);
	~GUI();

	virtual void Init();
	virtual void ProcessEntities(const EntityMap &entities);
	
private:
	ComponentMapper<GUIText> text_mapper_;
};

#endif // GUI_H