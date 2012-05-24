#ifndef ENTITY_H
#define ENTITY_H

/*
	remarks:	Entity stores information about what EntitySystems it belongs
				to and what Components it has in the form of bits in a bit
				string. Entities have both an id and a unique_id. The id is 
				used to map id to active Entity in EntityManager but because
				of object resuse, this id is not always unique. Unique_id is 
				used to represent the uniqueness of new Entities.
*/
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

	/*
		remarks:	Enabling/disabling an Entity is not fully implemented yet,
					leave Entities enabled.
	*/
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