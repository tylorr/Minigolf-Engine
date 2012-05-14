#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
	Entity();
	Entity(const unsigned int &id);

	void Reset();

	void AddTypeBit(const long &bit);
	void RemoveTypeBit(const long &bit);
	void AddSystemBit(const long &bit);
	void RemoveSystemBit(const long &bit);

	unsigned int id() {
		return id_;
	}

	long system_bits() {
		return system_bits_;
	}

	long type_bits() {
		return type_bits_;
	}

	bool enabled() {
		return enabled_;
	}

	void set_enabled(const bool &enabled) {
		enabled_ = enabled;
	}

private:
	static unsigned int next_unique_id_;

	bool enabled_;

	const unsigned int id_;
	unsigned int unique_id_;

	long system_bits_;
	long type_bits_;
};

#endif // ENTITY_H