#pragma once

#include <set>
#include <unordered_map>
#include <fstream>

#include "Entity_abstracts.h"
#include "pair_hash.h"

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <pplx/pplxtasks.h>
#include <concurrent_unordered_map.h>

#include <random>

class Entity;

inline std::default_random_engine generator;
inline std::uniform_int_distribution<int> distribution(1, 100);

inline auto roll_dice = []{return distribution(generator); };

#define MAX_ENTITIES 100

class World
{
	std::unordered_multimap<std::pair<int, int>, Entity*, pair_hash>* world_map;
	//std::unordered_set<Entity*>* removed_entities;

	std::vector<std::pair<Entity*,pplx::task<void>>> entity_tasks;

	pplx::cancellation_token_source cts;
	Concurrency::cancellation_token token = cts.get_token();

	//concurrency::concurrent_unordered_multimap<std::pair<int,int>,Entity*,pair_hash> world_map_conc;

	unsigned short ticks = 10;
protected:

public:
	World()
	{
		world_map = new std::unordered_multimap<std::pair<int, int>, Entity*, pair_hash>;
		world_map->reserve(100);

#ifdef _DEBUG
		std::cout << "World created" << std::endl;
#endif
	}
	~World()
	{
		for (auto& [pos, ent] : *world_map)
		{
			delete ent;
		}

		#ifdef _DEBUG
			std::cout << "World destroyed" << std::endl;
		#endif
	}

	void add_entity(Entity* org, std::pair<int, int> pos) 
	{
		world_map->insert(std::make_pair(pos, org));
#ifdef _DEBUG
		std::cout << "Entity added" << std::endl;
#endif
	}

	/*void remove_entity(Entity* org)
	{
#ifdef _DEBUG
		std::cout << "Entity removed" << std::endl;
#endif

	}*/

	bool move_entity(Entity* org,const std::pair<int, int> new_pos)
	{
		if(world_map->contains(new_pos) && world_map->find(new_pos)->second->is_solid()) return false;

		auto values_it =world_map->equal_range(org->get_position());
		for(auto it = values_it.first; it != values_it.second; it++)
		{
			if(org == it->second)
			{
				auto ptmp = world_map->extract(org->get_position());
				ptmp.mapped()->move(new_pos);
				ptmp.key() = new_pos;

				world_map->insert(std::move(ptmp));

#ifdef _DEBUG
				std::cout << "Entity moved to " << new_pos.first << " " << new_pos.second << std::endl;
#endif

				return true;

			}
		}

		return false;


	}

	Entity* get_entity(const std::pair<int, int> pos) const
	{
		return world_map->contains(pos) ? world_map->find(pos)->second :nullptr;
	}

	Entity* operator[](const std::pair<int, int> pos) const
	{
		return get_entity(pos);
	}

	void start()
	{
		for (auto& [pos, ent] : *world_map)
		{
			entity_tasks.emplace_back(ent, pplx::create_task([pos,&ent]()
			{
				Animal* an = dynamic_cast<Animal*>(ent);
				if(an == nullptr) return;

				std::this_thread::sleep_for(std::chrono::milliseconds(an->get_stats().initiative*1000));

				roll_dice();

			},token));
			
		}
	}

	void stop()
	{
		cts.cancel();
	}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar&* world_map;
	}
};