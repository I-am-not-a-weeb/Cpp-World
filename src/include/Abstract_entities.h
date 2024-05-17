#pragma once

#define MAX_HEALTH 100

#include <utility>
#include <list>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/utility.hpp>

#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>

#include "enums.h"


class Organism;

#include "World.h"
#include "Structs.h"
class World;



class Entity
{
	std::pair<int, int> position_;
	
protected:
	World* world_;
	bool solid_{false};

public:
	Entity() = default;
	explicit Entity(World* world,const std::pair<int, int>& pos) : position_(pos), world_(world)
	{
		
	}
	//virtual ~Entity() = default;
	std::pair<int, int> get_position() const
	{
		return position_;
	}
	void set_position(const std::pair<int, int>& pos)
	{
		position_ = pos;

		#ifdef _DEBUG
		std::cout << "Entity set new pos " << pos.first << " " << pos.second << std::endl;
		#endif

	}
	virtual bool move(std::pair<int, int> new_position) = 0;
	bool is_solid() const
	{
		return solid_;
	}
	virtual char ident() = 0;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& position_;
		ar& solid_;
	}
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Entity);



class Organism : public Entity
{
	const static int a;
protected:
	statistics stats;
	blood_line bloodline;


public:
	Organism() : Entity(), stats({ 1,0,MAX_HEALTH,0,0,0 })
	{
	
	}

	Organism(
		World* world,
		const std::pair<int, int>& pos,
		const statistics stats
	) : Entity(world,pos), stats(stats) {}

	Organism(
		World* world,
		const std::pair<int, int>& pos,
		const statistics stats,
		Organism* parent1,
		Organism* parent2
	) : Entity(world,pos), stats(stats), bloodline(parent1, parent2) {}

	void age_up()
	{
		stats.age++;
		stats.time_since_reproduce++;
	}
	bool recently_reproduced() const
	{
		if(stats.time_since_reproduce < 30)
		{
			return true;
		}
		return false;
	}
	bool get_gender() const
	{
		return stats.gender;
	}
	bool is_alive() const
	{
		return stats.alive;
	}
	statistics get_stats() const
	{
		return this->stats;
	}
	void suffer_damage(const int& damage)
	{
		stats.health -= damage;

#ifdef _DEBUG
		std::cout << "Organism suffers damage "<< damage << std::endl;
#endif

	}
	void set_health(const int& health)
	{
		stats.health = health;

#ifdef _DEBUG
		std::cout << "Organism got set health  " << health << std::endl;
#endif
	}
	virtual OrganismType get_type() = 0;
	virtual Organism* bear_child(Organism* parent2) = 0;
	virtual bool move(std::pair<int, int> new_position) = 0;
	void kill()
	{
		this->stats.alive = false;
	}
	virtual char ident() = 0;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Entity>(*this);
		ar& stats;
		ar& bloodline;
	}
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Organism);




class Animal : public Organism
{
protected:
	
public:
	Animal() = default;

	Animal(
		World* world,
		const std::pair<int, int>& pos,
		const statistics stats
	) : Organism(world,pos,stats)  {}

	Animal(
		World* world,
		const std::pair<int, int>& pos,
		const statistics stats,
		Animal* parent1,
		Animal* parent2
	) : Organism(world, pos, stats, parent1,parent2){}

	bool move(
		std::pair<int, int> new_position
	) override
	{
		set_position(new_position);

#ifdef _DEBUG
		std::cout << "Animal moved to " << new_position.first << " " << new_position.second <<std::endl;
#endif

		return true;
	}

	Organism* bear_child(
		Organism* parent2
	) = 0;


	virtual OrganismType get_type() = 0;
	virtual char ident() = 0;
private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Organism>(*this);
		//ar& health_points;
		//ar& bloodline;
	}

	/*template<class Archive>
	inline void load_construct_data(Archive& ar)
	{
		::new(this)Animal();
	}*/
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Animal);




class Plant : public Organism
{
private:
	
public:
	Plant() : Organism()
	{
		solid_ = true;
	}
	Plant(
		World* world,
		const std::pair<int, int>& pos,
		const statistics stats
	) : Organism(world, pos, stats)
	{
		solid_ = true;
	}
	Organism* bear_child(
		Organism* parent2
	) override { return nullptr; }

	bool move(std::pair<int, int> new_position) override
	{
#ifdef _DEBUG
		std::cout << "Tried to move plant... " << std::endl;
#endif

		return false;
	}
	/*{
		Organism* new_plant = new Plant();
		new_plant->bloodline = blood_line(parent1, parent2);
		this->bloodline.children.push_back(new_plant);


		return new_plant;
	}*/

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Organism>(*this);
		//ar& health_points;
		//ar& bloodline;
	}
};




BOOST_CLASS_EXPORT_GUID(Organism,"Organism")
BOOST_CLASS_EXPORT_GUID(Animal,"Animal")
BOOST_CLASS_EXPORT_GUID(Plant, "Plant")