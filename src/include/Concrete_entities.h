#pragma once

#include "Abstract_entities.h"
#include "Entity_abstracts.h"
#include "randomness.h"


namespace boost::serialization
{
	class access;
}

class Sheep : public Animal
{
public:
	Sheep(World* world,const std::pair<int, int>& pos) : Animal(world, pos, {(bool)roll_dice_2(), 12,0,1}) {}


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