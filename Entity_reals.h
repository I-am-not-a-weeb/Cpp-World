#pragma once

#include "Entity_abstracts.h"

class Sheep : public Animal
{
public:
	Sheep(): Animal(), stats({ 12,0,1 }) {}
	Sheep(const std::pair<int, int>& pos) : Animal(pos, { 12,0,1 }) {}


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Animal>(*this);
		//ar& health_points;
		//ar& bloodline;
	}
};

BOOST_CLASS_EXPORT_GUID(Sheep, "Sheep")