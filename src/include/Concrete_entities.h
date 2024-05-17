#pragma once

#include "Abstract_entities.h"
#include "Structs.h"
#include "randomness.h"

#include "enums.h"

namespace boost::serialization
{
	class access;
}

class Sheep : public Animal
{
	constexpr static OrganismType type = OrganismType::SHEEP;

public:
	Sheep() : Animal() {}
	Sheep(World* world,
		const std::pair<int, int>& pos) : Animal(world, pos, {1,(bool)roll_dice_2(), 8,0,4,0}) {}
	Sheep(World* world,
		const std::pair<int, int>& pos,
		const statistics stats) : Animal(world, pos, stats ) {}
	Sheep(World* world,
				const std::pair<int, int>& pos,
				const statistics stats,
				Sheep* parent1, Sheep* parent2) : Animal(world, pos, stats, parent1, parent2) {}

	Organism* bear_child(Organism* parent2) override
	{
		try
		{
			Sheep* tmp_parent2 = dynamic_cast<Sheep*>(parent2);

			if (tmp_parent2 == nullptr)
			{
				throw std::bad_cast();
			}

			Sheep* new_sheep = new Sheep(world_,
				this->get_position(),
				this->get_stats(),
				this, tmp_parent2
			);

			this->bloodline.children.push_back(new_sheep);

#ifdef _DEBUG
			std::cout << "Animal bears child" << std::endl;
#endif

			stats.time_since_reproduce = 0;

			return new_sheep;
		}
		catch (std::bad_cast& e)
		{
#ifdef _DEBUG
			std::cout << "Error bad_cast Animal: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
		catch (std::exception& e)
		{
#ifdef _DEBUG
			std::cout << "Error: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}

	}

	OrganismType get_type() override { return type; }
	char ident() override { return 'S'; }
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Animal>(*this);
		//ar& type;
	}
};
BOOST_CLASS_EXPORT_GUID(Sheep, "Sheep")


class Cow : public Animal
{
	constexpr static OrganismType type = OrganismType::COW;
public:
	Cow() : Animal() {}
	Cow(World* world,
		const std::pair<int, int>& pos) : Animal(world, pos, {1, (bool)roll_dice_2(), 12,0,6,0 }) {}
	Cow(World* world,
		const std::pair<int, int>& pos,
		const statistics stats) : Animal(world, pos, stats) {}
	Cow(World* world,
		const std::pair<int, int>& pos,
		const statistics stats,
		Cow* parent1, Cow* parent2) : Animal(world, pos, stats, parent1, parent2) {}

	OrganismType get_type() override { return type; }

	Organism* bear_child(Organism* parent2) override
	{
		try
		{
			Cow* tmp_parent2 = dynamic_cast<Cow*>(parent2);

			if (tmp_parent2 == nullptr)
			{
				throw std::bad_cast();
			}

			Cow* new_cow = new Cow(world_,
				this->get_position(),
				this->get_stats(),
				this, tmp_parent2
			);

			this->bloodline.children.push_back(new_cow);
#ifdef _DEBUG
			std::cout << "Animal bears child" << std::endl;
#endif

			stats.time_since_reproduce = 0;

			return new_cow;
		}
		catch (std::bad_cast& e)
		{
#ifdef _DEBUG
			std::cout << "Error bad_cast Animal: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
		catch (std::exception& e)
		{
#ifdef _DEBUG
			std::cout << "Error: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
		
	}
	char ident() override { return 'C'; }
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Animal>(*this);
		//ar& type;
	}
};
BOOST_CLASS_EXPORT_GUID(Cow, "Cow")

class Fox : public Animal
{
	constexpr static OrganismType type = OrganismType::FOX;
public:
	Fox() : Animal() {}
	Fox(World* world,
		const std::pair<int, int>& pos) : Animal(world, pos, {1, (bool)roll_dice_2(), 12,0,12,0 }) {}
	Fox(World* world,
		const std::pair<int, int>& pos,
		const statistics stats) : Animal(world, pos, stats) {}
	Fox(World* world,
		const std::pair<int, int>& pos,
		const statistics stats,
		Fox* parent1, Fox* parent2) : Animal(world, pos, stats, parent1, parent2){}

	OrganismType get_type() override { return type; }

	Organism* bear_child(Organism* parent2) override
	{
		try
		{
			Fox* tmp_parent2 = dynamic_cast<Fox*>(parent2);

			if (tmp_parent2 == nullptr)
			{
				throw std::bad_cast();
			}

			Fox* new_fox = new Fox(world_,
				this->get_position(),
				this->get_stats(),
				this, tmp_parent2
			);

			this->bloodline.children.push_back(new_fox);

#ifdef _DEBUG
			std::cout << "Animal bears child" << std::endl;
#endif

			stats.time_since_reproduce = 0;

			return new_fox;
		}
		catch (std::bad_cast& e)
		{
#ifdef _DEBUG
			std::cout << "Error bad_cast Animal: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
		catch (std::exception& e)
		{
#ifdef _DEBUG
			std::cout << "Error: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
	}
	char ident() override { return 'F'; }
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Animal>(*this);
		//ar& type;
	}
};
BOOST_CLASS_EXPORT_GUID(Fox, "Fox")

class Guarana : public Animal
{
	constexpr static OrganismType type = OrganismType::GUARANA;
public:
	Guarana() : Animal() {}
	Guarana(World* world,
		const std::pair<int, int>& pos) : Animal(world, pos, { 1, (bool)roll_dice_2(), 12,0,12,0 }) {}
	Guarana(World* world,
		const std::pair<int, int>& pos,
		const statistics stats) : Animal(world, pos, stats) {}
	Guarana(World* world,
		const std::pair<int, int>& pos,
		const statistics stats,
		Guarana* parent1, Guarana* parent2) : Animal(world, pos, stats, parent1, parent2) {}

	OrganismType get_type() override { return type; }

	Organism* bear_child(Organism* parent2) override
	{
		try
		{
			Guarana* tmp_parent2 = dynamic_cast<Guarana*>(parent2);

			if (tmp_parent2 == nullptr)
			{
				throw std::bad_cast();
			}

			Guarana* new_guarana = new Guarana(world_,
				this->get_position(),
				this->get_stats(),
				this, tmp_parent2
			);

			this->bloodline.children.push_back(new_guarana);

#ifdef _DEBUG
			std::cout << "Animal bears child" << std::endl;
#endif

			stats.time_since_reproduce = 0;

			return new_guarana;
		}
		catch (std::bad_cast& e)
		{
#ifdef _DEBUG
			std::cout << "Error bad_cast Guarana: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
		catch (std::exception& e)
		{
#ifdef _DEBUG
			std::cout << "Error: " << e.what() << std::endl;
#endif // DEBUG

			return nullptr;
		}
	}
	char ident() override { return 'G'; }
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Animal>(*this);
		//ar& type;
	}
};
BOOST_CLASS_EXPORT_GUID(Guarana, "Guarana")