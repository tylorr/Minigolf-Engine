#include "GL\glew.h"
#include "GL\freeglut.h"

#include "gui_text_render.h"
#include "gui_text.h"

GuiTextRender::GuiTextRender(const int &layer) : EntitySystem(layer) {
	TrackType<GuiText>();
}

GuiTextRender::~GuiTextRender() { }

void GuiTextRender::Init() { 
}

void GuiTextRender::ProcessEntities(const EntityMap &entities) {
	EntityMap::const_iterator it, ite;
	EntityPtr entity;
	GUITextPtr gui_text;

	float x, y;
	float h = 600.0f;
	float hw = 800 / 2.0f;
	float hh = h / 2.0f;

	for (it = entities.begin(), ite = entities.end(); it != ite; ++it) {
		entity = it->second;

		gui_text = text_mapper_(entity);

		const unsigned char *text = (const unsigned char *)gui_text->text.c_str();

		x = gui_text->position.x;
		y = gui_text->position.y;

		// offset to top left corner instead of bottom left
		y += 19.0f;

		// invert y-axis
		y = h - y;

		// offset origin
		x -= hw;
		y -= hh;

		// scale down to -1 to 1 range
		x /= hw;
		y /= hh;

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(x, y);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, text);
	}
}