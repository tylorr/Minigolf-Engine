#include <ctime>

#include "time.h"

namespace Time {

namespace {
	clock_t current = clock();
	clock_t previous = current;
	double time;
	float delta_time = 0;
}; // namespace

double GetTime() {
	return time;
}

float GetDeltaTime() {
	return delta_time;
}

void Update() {
	previous = current;
	current = clock();

	time = current / 1000.0;

	delta_time = (current - previous) / 1000.0f;
}

}; // namespace Time