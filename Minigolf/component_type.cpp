#include "component_type.h"

long ComponentType::next_bit_ = 1;
unsigned int ComponentType::next_id_ = 0;

ComponentType::ComponentType() {
	bit_ = next_bit_;
	next_bit_ <<= 1;
	id_ = next_id_++;
}

unsigned int ComponentType::id() {
	return id_;
}

long ComponentType::bit() {
	return bit_;
}