#include <algorithm>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include <boost\unordered_map.hpp>

#include "input.h"

namespace Input {

using boost::unordered_map;
using std::string;

namespace {
	unordered_map<string, bool> keys;
	unordered_map<string, bool> keyPrevious;

	std::string &ToLower(std::string &s) {
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}
}; // namespace 

void KeyPressed(unsigned char key, int x, int y) {
	string k;
	k.push_back(tolower(key));

	keys[k] = true;
}

void KeyReleased(unsigned char key, int x, int y) {
	string k;
	k.push_back(tolower(key));

	keys[k] = false;
}

void SpecialPressed(int Key, int X, int Y) {
	string k;
	switch(Key) {
	case GLUT_KEY_LEFT:
		k = "left";
		break;
	case GLUT_KEY_RIGHT:
		k = "right";
		break;
	case GLUT_KEY_UP:
		k = "up";
		break;
	case GLUT_KEY_DOWN:
		k = "down";
		break;
	}

	keyPrevious[k] = keys[k];
	keys[k] = true;
}

void SpecialReleased(int Key, int X, int Y) {
	string k;
	switch(Key) {
	case GLUT_KEY_LEFT:
		k = "left";
		break;
	case GLUT_KEY_RIGHT:
		k = "right";
		break;
	case GLUT_KEY_UP:
		k = "up";
		break;
	case GLUT_KEY_DOWN:
		k = "down";
		break;
	}

	keyPrevious[k] = keys[k];
	keys[k] = false;
}

bool GetKey(std::string key) {
	ToLower(key);
	return keys[key];
}

bool GetKeyDown(std::string key) {
	ToLower(key);

	return !keyPrevious[key] && keys[key];
}

bool GetKeyUp(std::string key) {
	ToLower(key);

	return keyPrevious[key] && !keys[key];
}

void Update() {
	unordered_map<string, bool>::iterator it;

	for (it = keyPrevious.begin(); it != keyPrevious.end(); ++it) {
		keyPrevious[it->first] = keys[it->first];
	}
}

}; // namespace Input