#include <vector>
#include <string>

#include <boost\unordered_map.hpp>

#include "system_manager.h"
#include "entity_system.h"

using std::vector;
using std::string;
using boost::shared_ptr;
using boost::unordered_map;

namespace SystemManager {

namespace {
	typedef shared_ptr<EntitySystem> SystemPtr;
	typedef vector<SystemPtr> SystemList;
	typedef unordered_map<string, long> SystemBitMap;

	SystemBitMap system_bits_;
	SystemList systems_; 
	long next_bit_ = 1;
}; // namespace

struct CompareSystems {
	bool operator()(SystemPtr lhs, SystemPtr rhs) { return *lhs < *rhs; }
};

void AddSystem(const SystemPtr &system) {
	using std::sort;

	systems_.push_back(system);
	system->set_system_bit(GetBitFor(system->family_name()));

	// keep systems sorted by layer
	sort(systems_.begin(), systems_.end(), CompareSystems());
}

long GetBitFor(const std::string &family_name) {
	SystemBitMap::iterator it;
	long bit;

	it = system_bits_.find(family_name);
	if (it == system_bits_.end()) {
		bit = next_bit_;
		next_bit_ <<= 1;
		system_bits_[family_name] = bit;
	} else {
		bit = it->second;
	}

	return bit;
}

void Refresh(const boost::shared_ptr<Entity> &entity) {
	SystemList::iterator it;

	for(it = systems_.begin(); it != systems_.end(); ++it) {
		(*it)->OnChange(entity);
	}
}

void Init() {
	SystemList::iterator it;

	for(it = systems_.begin(); it != systems_.end(); ++it) {
		(*it)->Init();
	}
}

void Resolve() {
	SystemList::iterator it;

	for(it = systems_.begin(); it != systems_.end(); ++it) {
		(*it)->Resolve();
	}
}

void Update() {
	SystemList::iterator it;

	for(it = systems_.begin(); it != systems_.end(); ++it) {
		(*it)->Process();
	}
}

}; // namespace SystemManager