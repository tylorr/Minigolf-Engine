#include <vector>

#include <boost\unordered_map.hpp>

#include "system_manager.h"
#include "entity_system.h"

using std::vector;
using boost::shared_ptr;
using boost::unordered_map;

namespace SystemManager {

namespace {
	typedef shared_ptr<EntitySystem> SystemPtr;
	typedef vector<SystemPtr> SystemList;
	typedef unordered_map<SystemPtr, long> SystemBitMap;

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
	system->set_system_bit(GetBitFor(system));

	// keep systems sorted by layer
	sort(systems_.begin(), systems_.end(), CompareSystems());
}

long GetBitFor(const SystemPtr &system) {
	SystemBitMap::iterator it;
	long bit;

	it = system_bits_.find(system);
	if (it == system_bits_.end()) {
		bit = next_bit_;
		next_bit_ <<= 1;
		system_bits_[system] = bit;
	} else {
		bit = it->second;
	}

	return bit;
}

}; // namespace SystemManager