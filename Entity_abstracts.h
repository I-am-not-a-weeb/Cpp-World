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


class Organism;

struct statistics
{
	bool gender;				// plec
	int health;					// zdrowie
	int age;					// liczba tikow decyzji
	const int initiative;		// szansa na pass

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& health;
		ar& age;
		ar& const_cast<int&>(initiative);
	}
};

struct blood_line
{
	Organism* parent1;
	Organism* parent2;
	std::list<Organism*> children;

	blood_line()
	{
		parent1 = nullptr;
		parent2 = nullptr;
	}
	blood_line(Organism* p1, Organism* p2) : parent1(p1), parent2(p2)
	{
		
	}

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& parent1;
		ar& parent2;
		ar& children;
	}
};



class Entity
{
	std::pair<int, int> position;
	
protected:
	bool solid_{false};

public:
	Entity() = default;
	explicit Entity(const std::pair<int, int>& pos) : position(pos)
	{
		
	}
	virtual ~Entity() = default;
	std::pair<int, int> get_position() const
	{
		return position;
	}
	void set_position(const std::pair<int, int>& pos)
	{
		position = pos;

#ifdef _DEBUG
		std::cout << "Entity set new pos " << pos.first << " " << pos.second << std::endl;
#endif

	}
	virtual bool move(std::pair<int, int> new_position) = 0;
	bool is_solid() const
	{
		return solid_;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& position;
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
	Organism() : Entity(), stats({ 0,MAX_HEALTH,0,0 })
	{

	}

	Organism(
		const std::pair<int, int>& pos,
		const statistics stats
	) : Entity(pos), stats(stats) {}

	Organism(
		const std::pair<int, int>& pos,
		const statistics stats,
		Organism* parent1,
		Organism* parent2
	) : Entity(pos), stats(stats), bloodline(parent1, parent2) {}

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

	virtual Organism* bear_child(Organism* parent2) = 0;
	virtual bool move(std::pair<int, int> new_position) = 0;

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
		const std::pair<int, int>& pos,
		const statistics stats
	) : Organism(pos,stats) {}

	Animal(
		const std::pair<int, int>& pos,
		const statistics stats,
		Animal* parent1,
		Animal* parent2
	) : Organism(pos, stats, parent1,parent2){}

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
	) override
	{
		try
		{
			Animal* new_animal = new Animal(
				this->get_position(),
				this->get_stats(),
				this,
				dynamic_cast<Animal*>(parent2));

			this->bloodline.children.push_back(new_animal);

#ifdef _DEBUG
			std::cout << "Animal bears child" << std::endl;
#endif

			return new_animal;
		}
		catch (std::bad_cast& e)
		{
			#ifdef _DEBUG
			std::cout << "Error bad_cast Animal: " << e.what() << std::endl;
			#endif // DEBUG

			return nullptr;
		}
		catch(std::exception& e)
		{
			#ifdef _DEBUG
			std::cout << "Error: " << e.what() << std::endl;
			#endif // DEBUG

			return nullptr;
		}

	}

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





class Plant : public Organism
{
private:
	
public:
	Plant() : Organism()
	{
		solid_ = true;
	}
	Plant(
		const std::pair<int, int>& pos,
		const statistics stats
	) : Organism(pos,stats)
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