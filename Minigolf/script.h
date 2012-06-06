#ifndef SCRIPT_H
#define SCRIPT_H

#include <boost\shared_ptr.hpp>

#include "component.h"

struct Script : public Component {
	const char *file;

	Script() : file("") { }
};

typedef boost::shared_ptr<Script> ScriptPtr;

#endif // SCRIPT_H