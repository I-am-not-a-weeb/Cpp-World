#pragma once
#include <list>
#include <boost/serialization/access.hpp>


class Organism;

struct statistics
{
	bool alive;
	bool gender;				// plec
	int health;					// zdrowie
	unsigned long age;			// liczba tikow decyzji
	const int initiative;		// szansa na pass
	unsigned int time_since_reproduce;	// czas od ostatniego rozmnazania

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
	blood_line(Organism* p1, Organism* p2) : parent1(p1), parent2(p2){}

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& parent1;
		ar& parent2;
		ar& children;
	}
};