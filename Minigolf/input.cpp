#include <algorithm>

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

	keyPrevious[k] = keys[k];
	keys[k] = true;
}

void KeyReleased(unsigned char key, int x, int y) {
	string k;
	k.push_back(tolower(key));

	keyPrevious[k] = keys[k];
	keys[k] = false;
}

void SpecialPressed(int Key, int X, int Y) {
}

void SpecialReleased(int Key, int X, int Y) {
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

}; // namespace Input