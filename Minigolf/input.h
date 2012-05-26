#ifndef INPUT_H
#define INPUT_H

#include <string>

namespace Input {

void KeyPressed(unsigned char, int, int);
void KeyReleased(unsigned char, int, int);
void SpecialPressed(int, int, int);
void SpecialReleased(int, int, int);

bool GetKey(std::string key);
bool GetKeyDown(std::string key);
bool GetKeyUp(std::string key);
void Update();

}; // namespace Input

#endif // INPUT_H